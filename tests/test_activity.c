#include <stdio.h>
#include <string.h>

#include "activity.h"
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

static void test_initialize_activity(void)
{
    Activity activity;

    initialize_activity(&activity, 25);

    check_test(
        activity.activity_id == 25,
        "initialize_activity sets activity ID"
    );

    check_test(
        activity.category == ACTIVITY_OTHER,
        "initialize_activity sets default category"
    );

    check_test(
        activity.classification == CLASS_ALIGNED,
        "initialize_activity sets default classification"
    );

    check_test(
        activity.distraction_reason == REASON_NONE,
        "initialize_activity sets no distraction reason"
    );
}

static void test_aligned_classification(void)
{
    Session session;

    initialize_session(&session, 1);

    session.original_intent = INTENT_STUDY;
    session.current_intent = INTENT_STUDY;

    check_test(
        classify_activity(
            &session,
            ACTIVITY_STUDY,
            0
        ) == CLASS_ALIGNED,
        "matching activity is classified as aligned"
    );
}

static void test_contextual_classification(void)
{
    Session session;

    initialize_session(&session, 2);

    session.original_intent = INTENT_STUDY;
    session.current_intent = INTENT_STUDY;

    check_test(
        classify_activity(
            &session,
            ACTIVITY_COMMUNICATION,
            1
        ) == CLASS_CONTEXTUAL,
        "related activity is classified as contextual"
    );
}

static void test_intent_change(void)
{
    Session session;
    ActivityClassification result;

    initialize_session(&session, 3);

    session.original_intent = INTENT_STUDY;
    session.current_intent = INTENT_STUDY;

    result = classify_activity(
        &session,
        ACTIVITY_WORK,
        2
    );

    check_test(
        result == CLASS_INTENT_CHANGE,
        "intentional switch is classified as intent change"
    );

    check_test(
        session.current_intent == INTENT_WORK,
        "intent change updates current intent"
    );

    check_test(
        session.original_intent == INTENT_STUDY,
        "intent change preserves original intent"
    );
}

static void test_drift_classification(void)
{
    Session session;

    initialize_session(&session, 4);

    session.original_intent = INTENT_STUDY;
    session.current_intent = INTENT_STUDY;

    check_test(
        classify_activity(
            &session,
            ACTIVITY_ENTERTAINMENT,
            3
        ) == CLASS_DRIFT,
        "unrelated activity is classified as drift"
    );
}

static void test_no_purpose_classification(void)
{
    Session session;

    initialize_session(&session, 5);

    session.original_intent = INTENT_NO_PURPOSE;
    session.current_intent = INTENT_NO_PURPOSE;

    check_test(
        classify_activity(
            &session,
            ACTIVITY_ENTERTAINMENT,
            0
        ) == CLASS_NO_PURPOSE,
        "no-purpose session uses no-purpose classification"
    );
}

static void test_context_storage(void)
{
    Activity activity;

    initialize_activity(&activity, 1);

    check_test(
        set_activity_context(
            &activity,
            "Opened after receiving a notification"
        ) == 1,
        "activity context can be stored"
    );

    check_test(
        strcmp(
            activity.context,
            "Opened after receiving a notification"
        ) == 0,
        "stored activity context is correct"
    );
}

static void test_distraction_reason(void)
{
    Activity activity;

    initialize_activity(&activity, 2);

    check_test(
        set_distraction_reason(
            &activity,
            REASON_NOTIFICATION
        ) == 1,
        "valid distraction reason is accepted"
    );

    check_test(
        activity.distraction_reason ==
            REASON_NOTIFICATION,
        "distraction reason is stored"
    );
}

static void test_add_activity(void)
{
    Session session;
    Activity activity;

    initialize_session(&session, 6);

    session.is_active = 1;

    initialize_activity(&activity, 1);

    activity.category = ACTIVITY_STUDY;
    activity.classification = CLASS_ALIGNED;

    check_test(
        add_activity_to_session(
            &session,
            &activity
        ) == 1,
        "activity can be added to active session"
    );

    check_test(
        session.activity_count == 1,
        "adding activity increments activity count"
    );

    check_test(
        session.aligned_count == 1,
        "aligned activity increments aligned count"
    );

    check_test(
        session.activities[0].activity_id == 1,
        "activity is stored in session array"
    );
}

static void test_reject_inactive_session(void)
{
    Session session;
    Activity activity;

    initialize_session(&session, 7);
    initialize_activity(&activity, 1);

    check_test(
        add_activity_to_session(
            &session,
            &activity
        ) == 0,
        "inactive session rejects activity"
    );
}

static void test_activity_timing(void)
{
    Activity activity;

    initialize_activity(&activity, 3);

    check_test(
        start_activity(&activity) == 1,
        "activity can be started"
    );

    check_test(
        activity.start_time != 0,
        "activity start time is recorded"
    );

    check_test(
        end_activity(&activity) == 1,
        "started activity can be ended"
    );

    check_test(
        activity.end_time != 0,
        "activity end time is recorded"
    );

    check_test(
        activity.duration_seconds >= 0,
        "activity duration is non-negative"
    );
}

int main(void)
{
    printf("====================================\n");
    printf("       WhyNow Activity Tests\n");
    printf("====================================\n\n");

    test_initialize_activity();
    test_aligned_classification();
    test_contextual_classification();
    test_intent_change();
    test_drift_classification();
    test_no_purpose_classification();
    test_context_storage();
    test_distraction_reason();
    test_add_activity();
    test_reject_inactive_session();
    test_activity_timing();

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