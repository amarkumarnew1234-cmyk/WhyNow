#ifndef REPORT_H
#define REPORT_H

#include <stddef.h>

#include "session.h"

typedef struct {
    size_t total_sessions;

    long total_duration_seconds;
    double average_duration_seconds;

    int total_activities;
    int total_aligned;
    int total_contextual;
    int total_intent_changes;
    int total_drifts;

    double average_alignment_percentage;
    double highest_alignment_percentage;
    double lowest_alignment_percentage;

    int sessions_with_goals;
    int completed_goals;

    DistractionReason most_common_distraction_reason;
    IntentType most_common_intent;
} HistoricalReport;

void initialize_historical_report(
    HistoricalReport *report
);

int generate_historical_report(
    const Session *sessions,
    size_t session_count,
    HistoricalReport *report
);

double calculate_goal_completion_percentage(
    const HistoricalReport *report
);

const char *report_dominant_pattern(
    const HistoricalReport *report
);

#endif
