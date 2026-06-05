#!/bin/sh
# run_benchmark.sh - WatchWeb/Agave benchmark regression runner.
#
# Runs every benchmark HTML page under the headless test harness with
# MALLOC_CHECK_=3 to catch heap corruption, and checks for crashes plus the
# RESULT:P/total summary each page emits via alert(). Exit non-zero if any
# page crashes or reports failures.
#
# Usage: ./run_benchmark.sh [path-to-headless_test]
#   default harness: ../proj/headless_test (relative to this script's dir)

set -u

DIR=$(cd "$(dirname "$0")" && pwd)
HEADLESS=${1:-"$DIR/../proj/headless_test"}
TIMEOUT_MS=8000

if [ ! -x "$HEADLESS" ]; then
    echo "ERROR: headless_test not found at: $HEADLESS"
    echo "Build it first, or pass its path as the first argument."
    exit 2
fi

# Correctness pages assert via T()/RESULT; perf pages just must not crash.
CORRECTNESS="dom_test refcount_dom refcount_events refcount_collections layout_prefwidth full_test"
PERF="perf_test full_perf"

rc=0

echo "== Correctness pages (no crash + all PASS) =="
for pg in $CORRECTNESS; do
    out=$(MALLOC_CHECK_=3 timeout 30 "$HEADLESS" "$DIR/$pg.html" $TIMEOUT_MS 2>&1)
    ec=$?
    result=$(printf '%s' "$out" | grep -oE 'RESULT:[0-9]+/[0-9]+' | head -1)
    crash=$(printf '%s' "$out" | grep -ciE 'ASSERT|Segmentation|corruption|Aborted')
    if [ "$ec" -eq 139 ] || [ "$ec" -eq 134 ] || [ "$crash" -ne 0 ]; then
        echo "  FAIL(crash) $pg ec=$ec"
        rc=1
    elif [ -n "$result" ]; then
        p=$(echo "$result" | cut -d: -f2 | cut -d/ -f1)
        t=$(echo "$result" | cut -d/ -f2)
        if [ "$p" = "$t" ]; then
            echo "  PASS $pg $result"
        else
            echo "  FAIL(asserts) $pg $result"
            rc=1
        fi
    else
        echo "  WARN $pg: no RESULT (ec=$ec)"
    fi
done

echo "== Perf pages (must not crash; timeout OK) =="
for pg in $PERF; do
    MALLOC_CHECK_=3 timeout 40 "$HEADLESS" "$DIR/$pg.html" 20000 >/tmp/_bench_perf.log 2>&1
    ec=$?
    crash=$(grep -ciE 'ASSERT|Segmentation|corruption|Aborted' /tmp/_bench_perf.log)
    if [ "$ec" -eq 139 ] || [ "$ec" -eq 134 ] || [ "$crash" -ne 0 ]; then
        echo "  FAIL(crash) $pg ec=$ec"
        rc=1
    else
        echo "  OK $pg (ec=$ec)"
    fi
done
rm -f /tmp/_bench_perf.log

if [ "$rc" -eq 0 ]; then
    echo "ALL BENCHMARK PAGES OK"
else
    echo "BENCHMARK REGRESSION DETECTED"
fi
exit $rc
