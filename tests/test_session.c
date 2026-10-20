#include <stdio.h>

#include "session.h"

static int tests_run = 0;
static int tests_passed = 0;

static void check_test(
    int condition,
    const char *test_name
)
{
    tests_run++;

    if (condition) {
        tests_passed++;
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s\n", test_name);
    }
}

static void test_initialize_session(void)
{
    Session session;

    initialize_session(&session, 10);

    check_test(
        session.session_id == 10,
        "initialize_session sets session ID"
    );

    check_test(
        session.activity_count == 0,
        "initialize_session resets activity count"
    );

    check_test(
        session.aligned_count == 0 &&
        session.contextual_count == 0 &&
        session.drift_count == 0 &&
        session.intent_change_count == 0,
        "initialize_session resets classification counters"
    );

    check_test(
        session.is_active == 0,
        "initialize_session creates inactive session"
    );
}

static void test_start_session(void)
{
    Session session;

    initialize_session(&session, 1);

    check_test(
        start_session(&session) == 1,
        "start_session starts inactive session"
    );

    check_test(
        session.is_active == 1,
        "start_session marks session active"
    );

    check_test(
        session.start_time != 0,
        "start_session records start time"
    );
}

static void test_prevent_second_start(void)
{
    Session session;

    initialize_session(&session, 2);

    start_session(&session);

    check_test(
        start_session(&session) == 0,
        "start_session prevents second active session"
    );
}

static void test_end_inactive_session(void)
{
    Session session;

    initialize_session(&session, 3);

    check_test(
        end_session(&session) == 0,
        "end_session rejects inactive session"
    );
}

static void test_end_active_session(void)
{
    Session session;

    initialize_session(&session, 4);
    start_session(&session);

    check_test(
        end_session(&session) == 1,
        "end_session ends active session"
    );

    check_test(
        session.is_active == 0,
        "end_session marks session inactive"
    );

    check_test(
        session.end_time != 0,
        "end_session records end time"
    );
}

static void test_alignment_percentage(void)
{
    Session session;
    double result;

    initialize_session(&session, 5);

    session.original_intent = INTENT_STUDY;

    session.aligned_count = 3;
    session.contextual_count = 1;
    session.intent_change_count = 0;
    session.drift_count = 1;

    result = calculate_alignment_percentage(&session);

    check_test(
        result > 79.99 && result < 80.01,
        "alignment calculation returns 80 percent"
    );
}

static void test_zero_activity_alignment(void)
{
    Session session;

    initialize_session(&session, 6);

    session.original_intent = INTENT_STUDY;

    check_test(
        calculate_alignment_percentage(&session) == 0.0,
        "alignment is zero when no activities exist"
    );
}

static void test_no_purpose_alignment(void)
{
    Session session;

    initialize_session(&session, 7);

    session.original_intent = INTENT_NO_PURPOSE;

    check_test(
        calculate_alignment_percentage(&session) == -1.0,
        "no-purpose session alignment is not applicable"
    );
}

int main(void)
{
    printf("====================================\n");
    printf("       WhyNow Session Tests\n");
    printf("====================================\n\n");

    test_initialize_session();
    test_start_session();
    test_prevent_second_start();
    test_end_inactive_session();
    test_end_active_session();
    test_alignment_percentage();
    test_zero_activity_alignment();
    test_no_purpose_alignment();

    printf("\n====================================\n");
    printf("Tests run   : %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n",
           tests_run - tests_passed);
    printf("====================================\n");

    if (tests_run == tests_passed) {
        printf("RESULT: ALL TESTS PASSED\n");
        return 0;
    }

    printf("RESULT: TEST FAILURE\n");

    return 1;
}