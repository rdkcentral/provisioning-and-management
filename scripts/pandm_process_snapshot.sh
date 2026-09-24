#!/bin/sh

OUTPUT=/tmp/pandm_process_snapshot.log
LOCKDIR=/tmp/.pandm_process_snapshot.lock
DURATION=10

if ! mkdir "$LOCKDIR" 2>/dev/null; then
    existingPid=$(cat "$LOCKDIR/pid" 2>/dev/null)
    if [ -n "$existingPid" ] && kill -0 "$existingPid" 2>/dev/null; then
        exit 0
    fi
    rm -f "$LOCKDIR/pid" 2>/dev/null
    rmdir "$LOCKDIR" 2>/dev/null || exit 0
    mkdir "$LOCKDIR" 2>/dev/null || exit 0
fi

printf '%s\n' "$$" > "$LOCKDIR/pid"
trap 'rm -f "$LOCKDIR/pid" 2>/dev/null; rmdir "$LOCKDIR" 2>/dev/null' 0 2 15

log_timestamp()
{
    date '+%Y-%m-%d %H:%M:%S'
}

snapshot_processes()
{
    stage=$1
    timestamp=$(log_timestamp)
    printf 'SNAPSHOT_BEGIN timestamp=%s stage=%s monitor_pid=%s\n' "$timestamp" "$stage" "$$" >> "$OUTPUT"

    for processPath in /proc/[0-9]*; do
        [ -d "$processPath" ] || continue
        processId=${processPath##*/}
        name=$(cat "$processPath/comm" 2>/dev/null) || continue
        status=$(cat "$processPath/status" 2>/dev/null) || continue
        parentPid=$(printf '%s\n' "$status" | sed -n 's/^PPid:[[:space:]]*//p')
        state=$(printf '%s\n' "$status" | sed -n 's/^State:[[:space:]]*//p')
        commandLine=$(tr '\000' ' ' < "$processPath/cmdline" 2>/dev/null)
        printf 'PROCESS pid=%s ppid=%s state=%s name=%s cmdline=%s\n' \
            "$processId" "$parentPid" "$state" "$name" "$commandLine" >> "$OUTPUT"
    done

    printf 'SNAPSHOT_END timestamp=%s stage=%s\n' "$(log_timestamp)" "$stage" >> "$OUTPUT"
}

printf 'MONITOR_START timestamp=%s pid=%s ppid=%s duration=%s\n' \
    "$(log_timestamp)" "$$" "$PPID" "$DURATION" >> "$OUTPUT"

interval=0
while [ "$interval" -lt "$DURATION" ]; do
    if [ "$interval" -eq 0 ]; then
        snapshot_processes after_gw_lan_refresh
    else
        snapshot_processes post_refresh_poll
    fi
    interval=$((interval + 1))
    [ "$interval" -lt "$DURATION" ] && sleep 1
done

printf 'MONITOR_COMPLETE timestamp=%s pid=%s\n' "$(log_timestamp)" "$$" >> "$OUTPUT"
exit 0
