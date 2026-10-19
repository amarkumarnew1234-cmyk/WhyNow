#include <string.h>
#include <time.h>

#include "session.h"

void initialize_session(Session *session, int session_id)
{
    if (session == NULL) {
        return;
    }

    memset(session, 0, sizeof(Session));

    session->session_id = session_id;
    session->original_intent = INTENT_STUDY;
    session->current_intent = INTENT_STUDY;
    session->device_type = DEVICE_OTHER;

    session->goal_status = GOAL_NOT_COMPLETED;
    session->satisfaction = NOT_SATISFIED;

    session->alignment_percentage = 0.0;
    session->is_active = 0;
}

int start_session(Session *session)
{
    if (session == NULL) {
        return 0;
    }

    if (session->is_active) {
        return 0;
    }

    session->start_time = time(NULL);

    if (session->start_time == (time_t)-1) {
        return 0;
    }

    session->end_time = 0;
    session->actual_duration_seconds = 0;
    session->is_active = 1;

    return 1;
}

int end_session(Session *session)
{
    if (session == NULL) {
        return 0;
    }

    if (!session->is_active) {
        return 0;
    }

    session->end_time = time(NULL);

    if (session->end_time == (time_t)-1) {
        return 0;
    }

    session->actual_duration_seconds =
        (long)difftime(session->end_time, session->start_time);

    session->alignment_percentage =
        calculate_alignment_percentage(session);

    session->is_active = 0;

    return 1;
}

double calculate_alignment_percentage(const Session *session)
{
    int classified_activities;
    int aligned_equivalent;

    if (session == NULL) {
        return 0.0;
    }

    if (session->original_intent == INTENT_NO_PURPOSE) {
        return -1.0;
    }

    classified_activities =
        session->aligned_count +
        session->contextual_count +
        session->intent_change_count +
        session->drift_count;

    if (classified_activities == 0) {
        return 0.0;
    }

    aligned_equivalent =
        session->aligned_count +
        session->contextual_count +
        session->intent_change_count;

    return ((double)aligned_equivalent /
            (double)classified_activities) * 100.0;
}
