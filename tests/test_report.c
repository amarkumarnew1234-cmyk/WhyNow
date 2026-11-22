#include <stdio.h>
#include <string.h>

#include "report.h"
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
    HistoricalReport report;
    Session sessions[3];

    printf(
        "====================================\n"
        "       WhyNow Reporting Tests\n"
        "====================================\n\n"
    );

    initialize_historical_report(&report);

    check_test(
        report.total_sessions == 0,
        "initial report has zero sessions"
    );

    check_test(
        report.total_duration_seconds == 0,
        "initial report has zero duration"
    );

    check_test(
        report.total_activities == 0,
        "initial report has zero activities"
    );

    check_test(
        report.average_alignment_percentage == 0.0,
        "initial average alignment is zero"
    );

    check_test(
        report.most_common_distraction_reason ==
            REASON_NONE,
        "initial distraction reason is none"
    );

    check_test(
        generate_historical_report(
            NULL,
            3,
            &report
        ) == 0,
        "report rejects null session array"
    );

    check_test(
        generate_historical_report(
            sessions,
            0,
            &report
        ) == 0,
        "report rejects zero session count"
    );

    check_test(
        generate_historical_report(
            sessions,
            3,
            NULL
        ) == 0,
        "report rejects null report output"
    );

    initialize_session(&sessions[0], 101);

    sessions[0].original_intent = INTENT_STUDY;
    sessions[0].current_intent = INTENT_STUDY;
    sessions[0].actual_duration_seconds = 3600;
    sessions[0].activity_count = 4;
    sessions[0].aligned_count = 3;
    sessions[0].contextual_count = 0;
    sessions[0].intent_change_count = 0;
    sessions[0].drift_count = 1;
    sessions[0].alignment_percentage = 75.0;
    sessions[0].goal_status = GOAL_COMPLETED;

    sessions[0].activities[0].distraction_reason =
        REASON_NONE;

    sessions[0].activities[1].distraction_reason =
        REASON_NONE;

    sessions[0].activities[2].distraction_reason =
        REASON_NONE;

    sessions[0].activities[3].distraction_reason =
        REASON_NOTIFICATION;

    initialize_session(&sessions[1], 102);

    sessions[1].original_intent = INTENT_WORK;
    sessions[1].current_intent = INTENT_WORK;
    sessions[1].actual_duration_seconds = 1800;
    sessions[1].activity_count = 4;
    sessions[1].aligned_count = 2;
    sessions[1].contextual_count = 1;
    sessions[1].intent_change_count = 0;
    sessions[1].drift_count = 1;
    sessions[1].alignment_percentage = 50.0;
    sessions[1].goal_status = GOAL_PARTIAL;

    sessions[1].activities[0].distraction_reason =
        REASON_NONE;

    sessions[1].activities[1].distraction_reason =
        REASON_NONE;

    sessions[1].activities[2].distraction_reason =
        REASON_NONE;

    sessions[1].activities[3].distraction_reason =
        REASON_NOTIFICATION;

    initialize_session(&sessions[2], 103);

    sessions[2].original_intent =
        INTENT_NO_PURPOSE;

    sessions[2].current_intent =
        INTENT_NO_PURPOSE;

    sessions[2].actual_duration_seconds = 900;
    sessions[2].activity_count = 2;
    sessions[2].aligned_count = 0;
    sessions[2].contextual_count = 0;
    sessions[2].intent_change_count = 0;
    sessions[2].drift_count = 2;
    sessions[2].alignment_percentage = 0.0;

    sessions[2].activities[0].distraction_reason =
        REASON_BOREDOM;

    sessions[2].activities[1].distraction_reason =
        REASON_NOTIFICATION;

    check_test(
        generate_historical_report(
            sessions,
            3,
            &report
        ) == 1,
        "historical report generation succeeds"
    );

    check_test(
        report.total_sessions == 3,
        "total session count is calculated"
    );

    check_test(
        report.total_duration_seconds == 6300,
        "total duration is calculated"
    );

    check_test(
        report.average_duration_seconds == 2100.0,
        "average duration is calculated"
    );

    check_test(
        report.total_activities == 10,
        "total activity count is calculated"
    );

    check_test(
        report.total_aligned == 5,
        "total aligned count is calculated"
    );

    check_test(
        report.total_contextual == 1,
        "total contextual count is calculated"
    );

    check_test(
        report.total_intent_changes == 0,
        "total intent-change count is calculated"
    );

    check_test(
        report.total_drifts == 4,
        "total drift count is calculated"
    );

    check_test(
        report.average_alignment_percentage == 62.5,
        "average alignment is calculated"
    );

    check_test(
        report.highest_alignment_percentage == 75.0,
        "highest alignment is calculated"
    );

    check_test(
        report.lowest_alignment_percentage == 50.0,
        "lowest alignment is calculated"
    );

    check_test(
        report.sessions_with_goals == 2,
        "sessions with goals are counted"
    );

    check_test(
        report.completed_goals == 1,
        "completed goals are counted"
    );

    check_test(
        calculate_goal_completion_percentage(
            &report
        ) == 50.0,
        "goal completion percentage is calculated"
    );

    check_test(
        report.most_common_distraction_reason ==
            REASON_NOTIFICATION,
        "most common distraction is notification"
    );

    check_test(
        report.most_common_intent ==
            INTENT_STUDY,
        "most common intent uses deterministic tie handling"
    );

    check_test(
        strcmp(
            report_dominant_pattern(&report),
            "Mostly aligned"
        ) == 0,
        "dominant historical pattern is mostly aligned"
    );

    check_test(
        calculate_goal_completion_percentage(
            NULL
        ) == 0.0,
        "goal completion rejects null report"
    );

    initialize_historical_report(&report);

    check_test(
        strcmp(
            report_dominant_pattern(&report),
            "No activity pattern"
        ) == 0,
        "empty report has no activity pattern"
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
            "RESULT: ALL REPORTING TESTS PASSED\n"
        );
        return 0;
    }

    printf(
        "RESULT: REPORTING TESTS FAILED\n"
    );

    return 1;
}
