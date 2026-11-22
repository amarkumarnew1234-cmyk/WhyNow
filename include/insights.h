#ifndef INSIGHTS_H
#define INSIGHTS_H

#include "session.h"

typedef struct {
    int total_activities;
    int aligned_activities;
    int contextual_activities;
    int intent_changes;
    int drift_activities;

    double aligned_percentage;
    double contextual_percentage;
    double intent_change_percentage;
    double drift_percentage;

    DistractionReason most_common_distraction_reason;
} BehaviourInsights;

void initialize_behaviour_insights(
    BehaviourInsights *insights
);

int calculate_behaviour_insights(
    const Session *session,
    BehaviourInsights *insights
);

const char *dominant_behaviour_to_string(
    const BehaviourInsights *insights
);

#endif
