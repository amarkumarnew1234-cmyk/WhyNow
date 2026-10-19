#include <string.h>

#include "activity.h"

void initialize_activity(Activity *activity, int activity_id)
{
    if (activity == NULL) {
        return;
    }

    memset(activity, 0, sizeof(Activity));

    activity->activity_id = activity_id;
    activity->category = ACTIVITY_OTHER;
    activity->classification = CLASS_ALIGNED;
    activity->distraction_reason = REASON_NONE;
}

ActivityClassification classify_activity(
    Session *session,
    ActivityType activity_category,
    int relationship_choice
)
{
    if (session == NULL) {
        return CLASS_DRIFT;
    }

    if (session->original_intent == INTENT_NO_PURPOSE) {
        return CLASS_NO_PURPOSE;
    }

    if ((int)activity_category == (int)session->current_intent) {
        return CLASS_ALIGNED;
    }

    switch (relationship_choice) {
        case 1:
            return CLASS_CONTEXTUAL;

        case 2:
            session->current_intent = (IntentType)activity_category;
            return CLASS_INTENT_CHANGE;

        case 3:
            return CLASS_DRIFT;

        default:
            return CLASS_DRIFT;
    }
}

int add_activity_to_session(
    Session *session,
    const Activity *activity
)
{
    Activity *destination;

    if (session == NULL || activity == NULL) {
        return 0;
    }

    if (!session->is_active) {
        return 0;
    }

    if (session->activity_count >= MAX_ACTIVITIES) {
        return 0;
    }

    destination = &session->activities[session->activity_count];
    *destination = *activity;

    session->activity_count++;

    switch (activity->classification) {
        case CLASS_ALIGNED:
            session->aligned_count++;
            break;

        case CLASS_CONTEXTUAL:
            session->contextual_count++;
            break;

        case CLASS_INTENT_CHANGE:
            session->intent_change_count++;
            break;

        case CLASS_DRIFT:
            session->drift_count++;
            break;

        case CLASS_NO_PURPOSE:
            break;
    }

    return 1;
}

const char *activity_type_to_string(ActivityType type)
{
    switch (type) {
        case ACTIVITY_STUDY:
            return "Study";

        case ACTIVITY_WORK:
            return "Work";

        case ACTIVITY_COMMUNICATION:
            return "Communication";

        case ACTIVITY_ENTERTAINMENT:
            return "Entertainment";

        case ACTIVITY_PERSONAL:
            return "Personal Task";

        case ACTIVITY_OTHER:
            return "Other";

        default:
            return "Unknown";
    }
}

const char *classification_to_string(
    ActivityClassification classification
)
{
    switch (classification) {
        case CLASS_ALIGNED:
            return "Aligned";

        case CLASS_CONTEXTUAL:
            return "Contextual";

        case CLASS_INTENT_CHANGE:
            return "Intent Change";

        case CLASS_DRIFT:
            return "Drift";

        case CLASS_NO_PURPOSE:
            return "No Purpose";

        default:
            return "Unknown";
    }
}

const char *distraction_reason_to_string(
    DistractionReason reason
)
{
    switch (reason) {
        case REASON_NONE:
            return "None";

        case REASON_NOTIFICATION:
            return "Notification";

        case REASON_BOREDOM:
            return "Boredom";

        case REASON_HABIT:
            return "Habit";

        case REASON_CURIOSITY:
            return "Curiosity";

        case REASON_NEW_TASK:
            return "New Task";

        case REASON_PLANNED_BREAK:
            return "Planned Break";

        case REASON_OTHER:
            return "Other";

        default:
            return "Unknown";
    }
}