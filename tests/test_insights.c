#include <math.h>
#include <stdio.h>
#include <string.h>

#include "activity.h"
#include "insights.h"
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

static int double_equals(
    double first,
    double second
)
{
    return fabs(first - second) < 0.001;
}

int main(void)
{
    Session session;
    BehaviourInsights insights;

    printf(
        "====================================\n"
        "     WhyNow Behaviour Insights Tests\n"
        "====================================\n\n"
    );

    initialize_behaviour_insights(&insights);

    check_test(
        insights.total_activities == 0,
        "initial insights have zero total activities"
    );

    check_test(
        insights.aligned_activities == 0,
        "initial aligned count is zero"
    );

    check_test(
        insights.contextual_activities == 0,
        "initial contextual count is zero"
    );

    check_test(
        insights.intent_changes == 0,
        "initial intent-change count is zero"
    );

    check_test(
        insights.drift_activities == 0,
        "initial drift count is zero"
    );

    check_test(
        double_equals(
            insights.aligned_percentage,
            0.0
        ),
        "initial aligned percentage is zero"
    );

    initialize_session(&session, 101);

    session.activity_count = 4;
    session.aligned_count = 2;
    session.contextual_count = 1;
    session.intent_change_count = 0;
    session.drift_count = 1;

    session.activities[0].classification =
        CLASS_ALIGNED;

    session.activities[1].classification =
        CLASS_ALIGNED;

    session.activities[2].classification =
        CLASS_CONTEXTUAL;

    session.activities[3].classification =
        CLASS_DRIFT;

    session.activities[3].distraction_reason =
        REASON_NOTIFICATION;

    check_test(
        calculate_behaviour_insights(
            &session,
            &insights
        ) == 1,
        "behaviour insights calculation succeeds"
    );

    check_test(
        insights.total_activities == 4,
        "total activity count is calculated"
    );

    check_test(
        insights.aligned_activities == 2,
        "aligned activity count is calculated"
    );

    check_test(
        insights.contextual_activities == 1,
        "contextual activity count is calculated"
    );

    check_test(
        insights.intent_changes == 0,
        "intent-change count is calculated"
    );

    check_test(
        insights.drift_activities == 1,
        "drift activity count is calculated"
    );

    check_test(
        double_equals(
            insights.aligned_percentage,
            50.0
        ),
        "aligned percentage is 50 percent"
    );

    check_test(
        double_equals(
            insights.contextual_percentage,
            25.0
        ),
        "contextual percentage is 25 percent"
    );

    check_test(
        double_equals(
            insights.intent_change_percentage,
            0.0
        ),
        "intent-change percentage is zero"
    );

    check_test(
        double_equals(
            insights.drift_percentage,
            25.0
        ),
        "drift percentage is 25 percent"
    );

    check_test(
        insights.most_common_distraction_reason ==
            REASON_NOTIFICATION,
        "most common distraction reason is notification"
    );

    check_test(
        strcmp(
            dominant_behaviour_to_string(
                &insights
            ),
            "Mostly Aligned"
        ) == 0,
        "dominant behaviour is mostly aligned"
    );

    check_test(
        calculate_behaviour_insights(
            NULL,
            &insights
        ) == 0,
        "null session is rejected"
    );

    check_test(
        calculate_behaviour_insights(
            &session,
            NULL
        ) == 0,
        "null insights output is rejected"
    );

    printf(
        "\n====================================\n"
    );
    printf(
        "Tests run   : %d\n",
        tests_run
    );
    printf(
        "Tests passed: %d\n",
        tests_passed
    );
    printf(
        "Tests failed: %d\n",
        tests_failed
    );
    printf(
        "====================================\n"
    );

    if (tests_failed == 0) {
        printf(
            "RESULT: ALL BEHAVIOUR INSIGHTS TESTS PASSED\n"
        );
        return 0;
    }

    printf(
        "RESULT: BEHAVIOUR INSIGHTS TESTS FAILED\n"
    );

    return 1;
}
