#include <stddef.h>

#include "report.h"

void initialize_historical_report(
    HistoricalReport *report
)
{
    if (report == NULL) {
        return;
    }

    report->total_sessions = 0;

    report->total_duration_seconds = 0;
    report->average_duration_seconds = 0.0;

    report->total_activities = 0;
    report->total_aligned = 0;
    report->total_contextual = 0;
    report->total_intent_changes = 0;
    report->total_drifts = 0;

    report->average_alignment_percentage = 0.0;
    report->highest_alignment_percentage = 0.0;
    report->lowest_alignment_percentage = 0.0;

    report->sessions_with_goals = 0;
    report->completed_goals = 0;

    report->most_common_distraction_reason =
        REASON_NONE;

    report->most_common_intent =
        INTENT_NO_PURPOSE;
}

int generate_historical_report(
    const Session *sessions,
    size_t session_count,
    HistoricalReport *report
)
{
    size_t index;
    int distraction_counts[REASON_OTHER + 1] = {0};
    int intent_counts[INTENT_NO_PURPOSE + 1] = {0};

    double alignment_total = 0.0;
    size_t alignment_session_count = 0;

    int highest_distraction_count = 0;
    int highest_intent_count = 0;

    if (sessions == NULL ||
        report == NULL ||
        session_count == 0) {
        return 0;
    }

    initialize_historical_report(report);

    report->total_sessions = session_count;

    for (index = 0; index < session_count; index++) {
        const Session *session = &sessions[index];
        int activity_index;

        report->total_duration_seconds +=
            session->actual_duration_seconds;

        report->total_activities +=
            session->activity_count;

        report->total_aligned +=
            session->aligned_count;

        report->total_contextual +=
            session->contextual_count;

        report->total_intent_changes +=
            session->intent_change_count;

        report->total_drifts +=
            session->drift_count;

        if (session->original_intent !=
            INTENT_NO_PURPOSE) {

            alignment_total +=
                session->alignment_percentage;

            alignment_session_count++;

            if (alignment_session_count == 1 ||
                session->alignment_percentage >
                    report->highest_alignment_percentage) {

                report->highest_alignment_percentage =
                    session->alignment_percentage;
            }

            if (alignment_session_count == 1 ||
                session->alignment_percentage <
                    report->lowest_alignment_percentage) {

                report->lowest_alignment_percentage =
                    session->alignment_percentage;
            }

            report->sessions_with_goals++;

            if (session->goal_status ==
                GOAL_COMPLETED) {
                report->completed_goals++;
            }
        }

        if (session->original_intent >=
                INTENT_STUDY &&
            session->original_intent <=
                INTENT_NO_PURPOSE) {

            intent_counts[
                session->original_intent
            ]++;
        }

        for (
            activity_index = 0;
            activity_index < session->activity_count;
            activity_index++
        ) {
            DistractionReason reason =
                session->activities[
                    activity_index
                ].distraction_reason;

            if (reason > REASON_NONE &&
                reason <= REASON_OTHER) {

                distraction_counts[reason]++;
            }
        }
    }

    report->average_duration_seconds =
        (double)report->total_duration_seconds /
        (double)session_count;

    if (alignment_session_count > 0) {
        report->average_alignment_percentage =
            alignment_total /
            (double)alignment_session_count;
    }

    for (
        index = REASON_NOTIFICATION;
        index <= REASON_OTHER;
        index++
    ) {
        if (distraction_counts[index] >
            highest_distraction_count) {

            highest_distraction_count =
                distraction_counts[index];

            report->most_common_distraction_reason =
                (DistractionReason)index;
        }
    }

    for (
        index = INTENT_STUDY;
        index <= INTENT_NO_PURPOSE;
        index++
    ) {
        if (intent_counts[index] >
            highest_intent_count) {

            highest_intent_count =
                intent_counts[index];

            report->most_common_intent =
                (IntentType)index;
        }
    }

    return 1;
}

double calculate_goal_completion_percentage(
    const HistoricalReport *report
)
{
    if (report == NULL ||
        report->sessions_with_goals == 0) {
        return 0.0;
    }

    return (
        (double)report->completed_goals * 100.0
    ) / (double)report->sessions_with_goals;
}

const char *report_dominant_pattern(
    const HistoricalReport *report
)
{
    if (report == NULL ||
        report->total_activities == 0) {
        return "No activity pattern";
    }

    if (report->total_aligned >=
            report->total_contextual &&
        report->total_aligned >=
            report->total_intent_changes &&
        report->total_aligned >=
            report->total_drifts) {

        return "Mostly aligned";
    }

    if (report->total_contextual >=
            report->total_intent_changes &&
        report->total_contextual >=
            report->total_drifts) {

        return "Mostly contextual";
    }

    if (report->total_intent_changes >=
        report->total_drifts) {

        return "Frequent intentional switching";
    }

    return "Frequent drift";
}
