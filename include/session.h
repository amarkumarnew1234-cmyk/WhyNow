#ifndef SESSION_H
#define SESSION_H

#include "whynow.h"

typedef struct {
    int session_id;

    IntentType original_intent;
    IntentType current_intent;

    DeviceType device_type;

    char goal[MAX_GOAL_LENGTH];
    int expected_duration_minutes;

    time_t start_time;
    time_t end_time;
    long actual_duration_seconds;

    Activity activities[MAX_ACTIVITIES];
    int activity_count;

    int aligned_count;
    int contextual_count;
    int drift_count;
    int intent_change_count;

    double alignment_percentage;

    GoalStatus goal_status;
    SessionSatisfaction satisfaction;

    char reflection[MAX_REFLECTION_LENGTH];

    int is_active;
} Session;

void initialize_session(Session *session, int session_id);

int start_session(Session *session);

int end_session(Session *session);

double calculate_alignment_percentage(const Session *session);

#endif
