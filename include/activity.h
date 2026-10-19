#ifndef ACTIVITY_H
#define ACTIVITY_H

#include "session.h"

void initialize_activity(Activity *activity, int activity_id);

ActivityClassification classify_activity(
    Session *session,
    ActivityType activity_category,
    int relationship_choice
);

int add_activity_to_session(
    Session *session,
    const Activity *activity
);

int start_activity(Activity *activity);

int end_activity(Activity *activity);

int set_activity_context(
    Activity *activity,
    const char *context
);

int set_distraction_reason(
    Activity *activity,
    DistractionReason reason
);

const char *activity_type_to_string(ActivityType type);

const char *classification_to_string(
    ActivityClassification classification
);

const char *distraction_reason_to_string(
    DistractionReason reason
);

#endif