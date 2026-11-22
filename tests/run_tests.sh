#!/bin/bash

set -e

SESSION_EXECUTABLE="/tmp/whynow_test_session"
ACTIVITY_EXECUTABLE="/tmp/whynow_test_activity"
INTEGRATION_EXECUTABLE="/tmp/whynow_test_integration"
INSIGHTS_EXECUTABLE="/tmp/whynow_test_insights"
STORAGE_EXECUTABLE="/tmp/whynow_test_storage"

cleanup()
{
    rm -f \
        "$SESSION_EXECUTABLE" \
        "$ACTIVITY_EXECUTABLE" \
        "$INTEGRATION_EXECUTABLE" \
        "$INSIGHTS_EXECUTABLE" \
        "$STORAGE_EXECUTABLE" \
        "test_sessions_storage.dat"
}

trap cleanup EXIT

echo "========================================"
echo "        WhyNow Automated Test Suite"
echo "========================================"

echo
echo "[BUILD] Compiling session tests..."

gcc -Wall -Wextra -Wpedantic -Werror \
    -Iinclude \
    tests/test_session.c \
    src/session.c \
    -o "$SESSION_EXECUTABLE"

echo "[PASS] Session tests compiled."

echo
echo "[TEST] Running session tests..."
"$SESSION_EXECUTABLE"

echo
echo "[BUILD] Compiling activity tests..."

gcc -Wall -Wextra -Wpedantic -Werror \
    -Iinclude \
    tests/test_activity.c \
    src/activity.c \
    src/session.c \
    -o "$ACTIVITY_EXECUTABLE"

echo "[PASS] Activity tests compiled."

echo
echo "[TEST] Running activity tests..."
"$ACTIVITY_EXECUTABLE"

echo
echo "[BUILD] Compiling integration tests..."

gcc -Wall -Wextra -Wpedantic -Werror \
    -Iinclude \
    tests/test_integration.c \
    src/session.c \
    src/activity.c \
    -o "$INTEGRATION_EXECUTABLE"

echo "[PASS] Integration tests compiled."

echo
echo "[TEST] Running integration tests..."
"$INTEGRATION_EXECUTABLE"

echo
echo "[BUILD] Compiling behaviour insights tests..."

gcc -Wall -Wextra -Wpedantic -Werror \
    -Iinclude \
    tests/test_insights.c \
    src/insights.c \
    src/session.c \
    src/activity.c \
    -o "$INSIGHTS_EXECUTABLE"

echo "[PASS] Behaviour insights tests compiled."

echo
echo "[TEST] Running behaviour insights tests..."
"$INSIGHTS_EXECUTABLE"

echo
echo "[BUILD] Compiling storage tests..."

gcc -Wall -Wextra -Wpedantic -Werror \
    -Iinclude \
    tests/test_storage.c \
    src/storage.c \
    src/session.c \
    -o "$STORAGE_EXECUTABLE"

echo "[PASS] Storage tests compiled."

echo
echo "[TEST] Running storage tests..."
"$STORAGE_EXECUTABLE"

echo
echo "========================================"
echo "        ALL TEST SUITES PASSED"
echo "        Total assertions: 103"
echo "========================================"

