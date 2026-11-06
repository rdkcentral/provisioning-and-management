#!/usr/bin/env python3
"""Host regression test for the port-forwarding WebConfig transaction/rollback.

Extracts the production functions from cosa_webconfig_api.c (and the PortMapping
IPv4 helper from cosa_nat_apis.c), builds them against an in-memory syscfg mock
with ASan/UBSan, and verifies that a rejected or failed portforwarding subdoc
leaves syscfg (including 1:1 NAT pfr_* entries) exactly as it was.

Usage: python3 source/test/portforwarding_rollback_test.py
"""
import os
import re
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
WEBCFG_C = os.path.join(ROOT, "source/TR-181/middle_layer_src/cosa_webconfig_api.c")
WEBCFG_H = os.path.join(ROOT, "source/TR-181/middle_layer_src/cosa_webconfig_api.h")
NAT_C = os.path.join(ROOT, "source-arm/TR-181/board_sbapi/cosa_nat_apis.c")

WEBCFG_FUNCS = [
    "clear_pf_cache", "clear_pf_cache_DB", "backup_pf_cache", "apply_pf_cache_ToDB",
    "CheckIfIpIsValid", "CheckIfPortsAreValid", "set_portmap_conf",
    "add_pf_snapshot_entry", "add_pf_snapshot_rule", "snapshot_pf_cache_from_DB",
    "init_pf_cache", "Process_PF_WebConfigRequest", "rollback_PortForwarding",
]


def extract(src, name):
    m = re.search(r"^[A-Za-z_][^;{}()]*\b%s\s*\([^;{]*\)\s*\{" % re.escape(name), src, re.M)
    if not m:
        sys.exit("cannot find function %s" % name)
    i, depth = m.end() - 1, 0
    while True:
        c = src[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return src[m.start():i + 1]
        i += 1


def header_defines():
    h = open(WEBCFG_H).read()
    keep = [l for l in h.splitlines()
            if re.match(r"#define (PORTMAP_CACHE_SIZE|BLOCK_SIZE|VAL_BLOCK_SIZE|ALIAS_\w+)\b", l)]
    return "\n".join(keep)


PRELUDE = r'''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

%(defines)s

typedef struct { char cmd[BLOCK_SIZE]; char val[VAL_BLOCK_SIZE]; } t_cache;
typedef struct { char *internal_client, *enable, *external_port_end_range, *protocol, *description, *external_port; } portdoc_t;
typedef struct { portdoc_t *entries; size_t entries_count; } portmappingdoc_t;
typedef struct { int ErrorCode; char ErrorMsg[128]; } Err, *pErr;
typedef int errno_t;
typedef unsigned long ULONG;

enum { BLOB_EXEC_SUCCESS = 0, VALID_IP = 0, SYSCFG_FAILURE = 301, INVALID_IP = 308, INVALID_PORT = 309, INVALID_PROTOCOL = 310 };
#define EOK 0
#define ERR_CHK(x) (void)(x)
#define CcspTraceError(x) printf x
#define CcspTraceWarning(x) printf x
#define CcspTraceInfo(x) printf x
#define UNREFERENCED_PARAMETER(x) (void)(x)

static int strcpy_s(char *d, size_t n, const char *s) { if (strlen(s) >= n) return -1; strcpy(d, s); return EOK; }
#define sprintf_s(d, n, ...) snprintf(d, n, __VA_ARGS__)
static int IsPortOverlapWithManagementAccess(int a, int b) { (void)a; (void)b; return 0; }

/* ---- in-memory syscfg ---- */
#define DBMAX 4096
static char dbk[DBMAX][64], dbv[DBMAX][VAL_BLOCK_SIZE];
static int dbn, writes, commits, restarts, fail_set_at = -1, sets_seen;
static void mk(char *o, const char *ns, const char *k) { if (ns) snprintf(o, 64, "%%s::%%s", ns, k); else snprintf(o, 64, "%%s", k); }
static int find(const char *k) { for (int i = 0; i < dbn; i++) if (!strcmp(dbk[i], k)) return i; return -1; }
int syscfg_get(const char *ns, const char *k, char *v, int n) { char f[64]; mk(f, ns, k); int i = find(f); if (i < 0) return -1; snprintf(v, n, "%%s", dbv[i]); return 0; }
int syscfg_set(const char *ns, const char *k, const char *v) {
    if (fail_set_at >= 0 && sets_seen++ == fail_set_at) return -1;
    char f[64]; mk(f, ns, k); int i = find(f); if (i < 0) { i = dbn++; strcpy(dbk[i], f); }
    snprintf(dbv[i], VAL_BLOCK_SIZE, "%%s", v); writes++; return 0; }
int syscfg_unset(const char *ns, const char *k) { char f[64]; mk(f, ns, k); int i = find(f); writes++; if (i < 0) return 0;
    dbn--; if (i != dbn) { memcpy(dbk[i], dbk[dbn], sizeof(dbk[i])); memcpy(dbv[i], dbv[dbn], sizeof(dbv[i])); } return 0; }
int syscfg_commit(void) { commits++; return 0; }
static int mock_system(const char *c) { if (strstr(c, "firewall-restart")) restarts++; return 0; }
#define system(x) mock_system(x)

t_cache pf_cache[PORTMAP_CACHE_SIZE];
t_cache pf_cache_bkup[PORTMAP_CACHE_SIZE];
static int pf_cache_size = 0;
static int pf_cache_size_bkup = 0;
static int gpfEnable = 0;
static int gpfEnable_bkup = 0;
static int pf_database_changed = 0;
'''

TESTS = r'''
static int failures;
#define CHECK(c) do { if (!(c)) { printf("FAIL %%s:%%d: %%s\n", __func__, __LINE__, #c); failures++; } } while (0)

static void db_reset(void) { dbn = 0; writes = commits = restarts = 0; fail_set_at = -1; sets_seen = 0; }
static void put(const char *k, const char *v) { syscfg_set(NULL, k, v); }
static const char *get(const char *k) { int i = find(k); return i < 0 ? NULL : dbv[i]; }

static char snapk[DBMAX][64], snapv[DBMAX][VAL_BLOCK_SIZE]; static int snapn;
static void snap(void) { snapn = dbn; memcpy(snapk, dbk, sizeof(dbk)); memcpy(snapv, dbv, sizeof(dbv)); }
static int same_as_snap(void) {
    if (snapn != dbn) { printf("  db size %%d != %%d\n", dbn, snapn); return 0; }
    for (int i = 0; i < snapn; i++) { const char *v = get(snapk[i]);
        if (!v || strcmp(v, snapv[i])) { printf("  key %%s: '%%s' != '%%s'\n", snapk[i], v ? v : "(absent)", snapv[i]); return 0; } }
    return 1; }

/* logs2: placeholder pfr_1 present at boot, later filled in by the UI with 1:1 NAT values */
static void seed_logs2(int filled) {
    db_reset();
    put("CosaNAT::port_forward_enabled", "1");
    put("SinglePortForwardCount", "0");
    put("PortRangeForwardCount", "1");
    put("PortRangeForward_1", "pfr_1");
    const char *k[] = { "::enabled", "::external_port_range", "::internal_port", "::internal_port_range_size",
                        "::name", "::prev_rule_enabled_state", "::protocol", "::public_ip", "::to_ip", "::to_ipv6" };
    const char *f[] = { "1", "0 0", "0", "0", "one2one", "1", "both", "73.241.194.89", "10.0.0.2", "x" };
    for (int i = 0; i < 10; i++) { char key[64]; snprintf(key, 64, "pfr_1%%s", k[i]); put(key, filled ? f[i] : (i == 0 ? "0" : "")); }
    writes = commits = restarts = 0;
}

static portdoc_t bad[3] = {
    { "10.0.0.5", "true", "0", "TCP", "a", "0" }, { "10.0.0.6", "true", "0", "TCP", "b", "0" }, { "10.0.0.7", "true", "0", "UDP", "c", "0" } };
static portdoc_t good[2] = { { "10.0.0.20", "true", "8080", "TCP", "web", "8080" }, { "10.0.0.21", "true", "6000", "both", "rng", "5000" } };

static void test_invalid_subdoc_after_ui_edit(void) {
    seed_logs2(0);
    CHECK(init_pf_cache(pf_cache) == 0);           /* boot */
    seed_logs2(1);                                 /* UI fills 1:1 NAT after boot */
    snap();
    portmappingdoc_t d = { bad, 3 };
    pErr e = Process_PF_WebConfigRequest(&d);
    CHECK(e && e->ErrorCode == INVALID_PORT);
    CHECK(writes == 0 && commits == 0 && restarts == 0);
    free(e);
    CHECK(rollback_PortForwarding() == 0);
    CHECK(writes == 0 && restarts == 0);
    CHECK(same_as_snap());
    CHECK(get("pfr_1::public_ip") && !strcmp(get("pfr_1::public_ip"), "73.241.194.89"));
}

static void test_failure_after_db_change_restores_exactly(void) {
    for (int at = 0; at < 30; at++) {
        seed_logs2(1);
        snap();
        portmappingdoc_t d = { good, 2 };
        fail_set_at = at;                           /* fail somewhere in apply */
        pErr e = Process_PF_WebConfigRequest(&d);
        int failed = e && e->ErrorCode == SYSCFG_FAILURE;
        free(e);
        fail_set_at = -1;
        if (!failed) continue;
        CHECK(rollback_PortForwarding() == 0);
        if (!same_as_snap()) { printf("  (fail_set_at=%%d)\n", at); failures++; }
    }
}

static void test_valid_and_empty_apply(void) {
    seed_logs2(1);
    portmappingdoc_t d = { good, 2 };
    pErr e = Process_PF_WebConfigRequest(&d);
    CHECK(e && e->ErrorCode == BLOB_EXEC_SUCCESS); free(e);
    CHECK(restarts == 1);
    CHECK(!strcmp(get("SinglePortForwardCount"), "1") && !strcmp(get("PortRangeForwardCount"), "1"));
    CHECK(!strcmp(get("PortRangeForward_1"), "pfr_2") && !strcmp(get("pfr_2::to_ip"), "10.0.0.21"));
    CHECK(get("pfr_1::public_ip") == NULL);         /* replaced, not merged */
    CHECK(!strcmp(get("CosaNAT::port_forward_enabled"), "1"));

    portmappingdoc_t empty = { NULL, 0 };
    e = Process_PF_WebConfigRequest(&empty);
    CHECK(e && e->ErrorCode == BLOB_EXEC_SUCCESS); free(e);
    CHECK(!strcmp(get("SinglePortForwardCount"), "0") && !strcmp(get("PortRangeForwardCount"), "0"));
    CHECK(get("SinglePortForward_1") == NULL && get("PortRangeForward_1") == NULL);
    CHECK(!strcmp(get("CosaNAT::port_forward_enabled"), "0"));
}

static void test_snapshot_does_not_create_missing_keys(void) {
    seed_logs2(1);
    syscfg_unset(NULL, "pfr_1::to_ipv6");
    db_reset(); seed_logs2(1); syscfg_unset(NULL, "pfr_1::to_ipv6"); writes = 0;
    snap();
    portmappingdoc_t d = { good, 2 };
    fail_set_at = 5;
    pErr e = Process_PF_WebConfigRequest(&d); free(e);
    fail_set_at = -1;
    CHECK(rollback_PortForwarding() == 0);
    CHECK(get("pfr_1::to_ipv6") == NULL);
    CHECK(same_as_snap());
}

static void test_snapshot_failures_leave_db_untouched(void) {
    char k[64], v[64];
    db_reset();
    put("SinglePortForwardCount", "0");
    put("PortRangeForwardCount", "200");             /* 200 * 11 > cache */
    for (int i = 1; i <= 200; i++) { snprintf(k, 64, "PortRangeForward_%%d", i); snprintf(v, 64, "pfr_%%d", i); put(k, v);
        snprintf(k, 64, "pfr_%%d::to_ip", i); put(k, "10.0.0.9"); snprintf(k, 64, "pfr_%%d::enabled", i); put(k, "1");
        snprintf(k, 64, "pfr_%%d::name", i); put(k, "n"); snprintf(k, 64, "pfr_%%d::protocol", i); put(k, "tcp");
        snprintf(k, 64, "pfr_%%d::external_port_range", i); put(k, "1 2"); snprintf(k, 64, "pfr_%%d::public_ip", i); put(k, "0.0.0.0"); }
    writes = 0; snap();
    portmappingdoc_t d = { good, 2 };
    pErr e = Process_PF_WebConfigRequest(&d);
    CHECK(e && e->ErrorCode == SYSCFG_FAILURE); free(e);
    CHECK(writes == 0 && restarts == 0);
    CHECK(rollback_PortForwarding() == 0 && writes == 0);
    CHECK(same_as_snap());

    db_reset();
    put("SinglePortForwardCount", "0"); put("PortRangeForwardCount", "1");
    put("PortRangeForward_1", "pfr_123456789012345");  /* alias + suffix exceeds BLOCK_SIZE */
    put("pfr_123456789012345::prev_rule_enabled_state", "1");
    t_cache *c = calloc(PORTMAP_CACHE_SIZE, sizeof(t_cache)); int n, en;
    CHECK(snapshot_pf_cache_from_DB(c, &n, &en) == -1);
    free(c);
}

static void test_too_many_entries(void) {
    seed_logs2(1); snap();
    size_t n = (PORTMAP_CACHE_SIZE - 2) / 11 + 1;
    portdoc_t *many = calloc(n, sizeof(portdoc_t));
    for (size_t i = 0; i < n; i++) many[i] = good[0];
    portmappingdoc_t d = { many, n };
    pErr e = Process_PF_WebConfigRequest(&d);
    CHECK(e && e->ErrorCode == SYSCFG_FAILURE); free(e);
    CHECK(writes == 0);
    CHECK(rollback_PortForwarding() == 0 && same_as_snap());
    free(many);
}

static void test_ipv4_helper(void) {
    CHECK(_PortMappingIPv4ToValue("", "f", 1) == 0);
    CHECK(_PortMappingIPv4ToValue(NULL, "f", 1) == 0);
    CHECK(_PortMappingIPv4ToValue("255.255.255.255", "f", 1) == 0);
    CHECK(_PortMappingIPv4ToValue("bogus", "f", 1) == 0);
    CHECK(_PortMappingIPv4ToValue("0.0.0.0", "f", 1) == 0);
    CHECK(_PortMappingIPv4ToValue("10.0.0.2", "f", 1) == inet_addr("10.0.0.2"));
}

int main(void) {
    test_invalid_subdoc_after_ui_edit();
    test_failure_after_db_change_restores_exactly();
    test_valid_and_empty_apply();
    test_snapshot_does_not_create_missing_keys();
    test_snapshot_failures_leave_db_untouched();
    test_too_many_entries();
    test_ipv4_helper();
    printf("\n%%s (%%d failure(s))\n", failures ? "FAILED" : "PASSED", failures);
    return failures != 0;
}
'''


def main():
    web = open(WEBCFG_C).read()
    nat = open(NAT_C).read()
    body = "\n\n".join(extract(web, f) for f in WEBCFG_FUNCS)
    helper = extract(nat, "_PortMappingIPv4ToValue")
    src = PRELUDE % {"defines": header_defines()} + helper + "\n\n" + body + "\n" + TESTS % {}

    cc = shutil.which("gcc") or shutil.which("cc")
    if not cc:
        sys.exit("no C compiler found")
    # /tmp may be mounted noexec, so build under the repository.
    work = tempfile.mkdtemp(prefix=".pf_rollback_test_", dir=ROOT)
    try:
        c_file, exe = os.path.join(work, "t.c"), os.path.join(work, "t")
        open(c_file, "w").write(src)
        flags = ["-std=gnu11", "-g", "-O1", "-Wall", "-Wno-format-truncation", "-Wno-unused-function",
                 "-fsanitize=address,undefined", "-fno-sanitize-recover=all"]
        r = subprocess.run([cc] + flags + [c_file, "-o", exe])
        if r.returncode:
            return r.returncode
        r = subprocess.run([exe], stdout=subprocess.PIPE, universal_newlines=True)
        lines = r.stdout.splitlines()
        print("\n".join(l for l in lines if l.startswith(("FAIL", "  ", "PASSED", "FAILED"))))
        return r.returncode
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
