#include <math.h>
#include <stdio.h>
#include <string.h>

#include "activity.h"
#include "session.h"

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

static void check_test(
    int condition,
    const char *description
)
{
    tests_run++;

    if (condition) {
        tests_passed++;
        printf("[PASS] %s\n", description);
    } else {
        tests_failed++;
        printf("[FAIL] %s\n", description);
    }
}

int main(void)
{
    Session session;
    Activity study_activity;
    Activity drift_activity;

    printf("====================================\n");
    printf("      WhyNow Integration Tests\n");
    printf("====================================\n\n");

    initialize_session(&session, 1);

    session.original_intent = INTENT_STUDY;
    session.current_intent = INTENT_STUDY;
    session.device_type = DEVICE_LAPTOP;

    strcpy(
        session.goal,
        "Prepare DAA dynamic programming"
    );

    session.expected_duration_minutes = 60;

    check_test(
        start_session(&session) == 1,
        "study session starts successfully"
    );

    check_test(
        session.is_active == 1,
        "session becomes active"
    );

    initialize_activity(&study_activity, 1);

    strcpy(
        study_activity.name,
        "DAA Dynamic Programming"
    );

    study_activity.category = ACTIVITY_STUDY;

    check_test(
        start_activity(&study_activity) == 1,
        "aligned activity starts successfully"
    );

    study_activity.classification =
        classify_activity(
            &session,
            study_activity.category,
            0
        );

    check_test(
        study_activity.classification == CLASS_ALIGNED,
        "study activity is classified as aligned"
    );

    check_test(
        set_activity_context(
            &study_activity,
            "Solving dynamic programming problems"
        ) == 1,
        "aligned activity context is stored"
    );

    check_test(
        end_activity(&study_activity) == 1,
        "aligned activity ends successfully"
    );

    check_test(
        add_activity_to_session(
            &session,
            &study_activity
        ) == 1,
        "aligned activity is added to session"
    );

    initialize_activity(&drift_activity, 2);

    strcpy(
        drift_activity.name,
        "Instagram"
    );

    drift_activity.category = ACTIVITY_ENTERTAINMENT;

    check_test(
        start_activity(&drift_activity) == 1,
        "drift activity starts successfully"
    );

    drift_activity.classification =
        classify_activity(
            &session,
            drift_activity.category,
            3
        );

    check_test(
        drift_activity.classification == CLASS_DRIFT,
        "entertainment activity is classified as drift"
    );

    check_test(
        set_distraction_reason(
            &drift_activity,
            REASON_NOTIFICATION
        ) == 1,
        "notification is stored as drift reason"
    );

    check_test(
        set_activity_context(
            &drift_activity,
            "Opened Instagram after a notification"
        ) == 1,
        "drift activity context is stored"
    );

    check_test(
        end_activity(&drift_activity) == 1,
        "drift activity ends successfully"
    );

    check_test(
        add_activity_to_session(
            &session,
            &drift_activity
        ) == 1,
        "drift activity is added to session"
    );

    check_test(
        session.activity_count == 2,
        "session contains two activities"
    );

    check_test(
        session.aligned_count == 1,
        "session contains one aligned activity"
    );

    check_test(
        session.drift_count == 1,
        "session contains one drift activity"
    );

    check_test(
        session.contextual_count == 0,
        "session contains no contextual activities"
    );

    check_test(
        session.intent_change_count == 0,
        "session contains no intent changes"
    );

    check_test(
        end_session(&session) == 1,
        "session ends successfully"
    );

    check_test(
        session.is_active == 0,
        "session becomes inactive after ending"
    );

    check_test(
        fabs(session.alignment_percentage - 50.0) < 0.001,
        "final intent alignment is 50 percent"
    );

    check_test(
        session.activities[0].classification == CLASS_ALIGNED,
        "first stored activity remains aligned"
    );

    check_test(
        session.activities[1].classification == CLASS_DRIFT,
        "second stored activity remains drift"
    );

    check_test(
        session.activities[1].distraction_reason ==
            REASON_NOTIFICATION,
        "stored drift reason remains notification"
    );

    printf("\n====================================\n");
    printf("Tests run   : %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    printf("====================================\n");

    if (tests_failed == 0) {
        printf("RESULT: ALL INTEGRATION TESTS PASSED\n");
        return 0;
    }

    printf("RESULT: INTEGRATION TESTS FAILED\n");

    return 1;
}