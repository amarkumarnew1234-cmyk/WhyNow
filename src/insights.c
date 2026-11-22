#include <stddef.h>

#include "insights.h"

void initialize_behaviour_insights(
    BehaviourInsights *insights
)
{
    if (insights == NULL) {
        return;
    }

    insights->total_activities = 0;
    insights->aligned_activities = 0;
    insights->contextual_activities = 0;
    insights->intent_changes = 0;
    insights->drift_activities = 0;

    insights->aligned_percentage = 0.0;
    insights->contextual_percentage = 0.0;
    insights->intent_change_percentage = 0.0;
    insights->drift_percentage = 0.0;

    insights->most_common_distraction_reason = REASON_NONE;
}

int calculate_behaviour_insights(
    const Session *session,
    BehaviourInsights *insights
)
{
    int reason_counts[REASON_OTHER + 1] = {0};
    int highest_reason_count = 0;
    int i;

    if (session == NULL || insights == NULL) {
        return 0;
    }

    initialize_behaviour_insights(insights);

    insights->total_activities = session->activity_count;
    insights->aligned_activities = session->aligned_count;
    insights->contextual_activities = session->contextual_count;
    insights->intent_changes = session->intent_change_count;
    insights->drift_activities = session->drift_count;

    if (insights->total_activities > 0) {
        insights->aligned_percentage =
            ((double)insights->aligned_activities /
             insights->total_activities) * 100.0;

        insights->contextual_percentage =
            ((double)insights->contextual_activities /
             insights->total_activities) * 100.0;

        insights->intent_change_percentage =
            ((double)insights->intent_changes /
             insights->total_activities) * 100.0;

        insights->drift_percentage =
            ((double)insights->drift_activities /
             insights->total_activities) * 100.0;
    }

    for (i = 0; i < session->activity_count; i++) {
        const Activity *activity = &session->activities[i];

        if (activity->classification == CLASS_DRIFT &&
            activity->distraction_reason >= REASON_NOTIFICATION &&
            activity->distraction_reason <= REASON_OTHER) {

            reason_counts[activity->distraction_reason]++;
        }
    }

    for (i = REASON_NOTIFICATION;
         i <= REASON_OTHER;
         i++) {

        if (reason_counts[i] > highest_reason_count) {
            highest_reason_count = reason_counts[i];

            insights->most_common_distraction_reason =
                (DistractionReason)i;
        }
    }

    return 1;
}

const char *dominant_behaviour_to_string(
    const BehaviourInsights *insights
)
{
    if (insights == NULL ||
        insights->total_activities == 0) {
        return "No activity data";
    }

    if (insights->aligned_activities >=
            insights->contextual_activities &&
        insights->aligned_activities >=
            insights->intent_changes &&
        insights->aligned_activities >=
            insights->drift_activities) {
        return "Mostly Aligned";
    }

    if (insights->contextual_activities >=
            insights->intent_changes &&
        insights->contextual_activities >=
            insights->drift_activities) {
        return "Mostly Contextual";
    }

    if (insights->intent_changes >=
        insights->drift_activities) {
        return "Frequent Intent Changes";
    }

    return "Drift Dominant";
}
