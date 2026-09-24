#!/bin/sh

OUTPUT=/tmp/pandm_process_snapshot.log
DURATION=10

printf 'MONITOR_START pid=%s ppid=%s duration=%s\n' "$$" "$PPID" "$DURATION" >> "$OUTPUT"

interval=0
while [ "$interval" -lt "$DURATION" ]; do
    printf 'SNAPSHOT_BEGIN interval=%s\n' "$interval" >> "$OUTPUT"
    ps w >> "$OUTPUT" 2>&1
    printf 'SNAPSHOT_END interval=%s\n' "$interval" >> "$OUTPUT"
    interval=$((interval + 1))
    [ "$interval" -lt "$DURATION" ] && sleep 1
done

printf 'MONITOR_COMPLETE pid=%s\n' "$$" >> "$OUTPUT"
exit 0
