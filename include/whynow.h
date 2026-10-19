#ifndef WHYNOW_H
#define WHYNOW_H

#include <time.h>

#define MAX_GOAL_LENGTH 200
#define MAX_NAME_LENGTH 100
#define MAX_CONTEXT_LENGTH 200
#define MAX_REFLECTION_LENGTH 200
#define MAX_ACTIVITIES 50

typedef enum {
    INTENT_STUDY = 1,
    INTENT_WORK,
    INTENT_COMMUNICATION,
    INTENT_ENTERTAINMENT,
    INTENT_PERSONAL,
    INTENT_NO_PURPOSE
} IntentType;

typedef enum {
    ACTIVITY_STUDY = 1,
    ACTIVITY_WORK,
    ACTIVITY_COMMUNICATION,
    ACTIVITY_ENTERTAINMENT,
    ACTIVITY_PERSONAL,
    ACTIVITY_OTHER
} ActivityType;

typedef enum {
    CLASS_ALIGNED = 0,
    CLASS_CONTEXTUAL,
    CLASS_INTENT_CHANGE,
    CLASS_DRIFT,
    CLASS_NO_PURPOSE
} ActivityClassification;

typedef enum {
    GOAL_COMPLETED = 1,
    GOAL_PARTIAL,
    GOAL_NOT_COMPLETED
} GoalStatus;

typedef enum {
    DEVICE_PHONE = 1,
    DEVICE_LAPTOP,
    DEVICE_TABLET,
    DEVICE_OTHER
} DeviceType;

typedef enum {
    REASON_NONE = 0,
    REASON_NOTIFICATION,
    REASON_BOREDOM,
    REASON_HABIT,
    REASON_CURIOSITY,
    REASON_NEW_TASK,
    REASON_PLANNED_BREAK,
    REASON_OTHER
} DistractionReason;

typedef enum {
    SATISFIED = 1,
    PARTIALLY_SATISFIED,
    NOT_SATISFIED
} SessionSatisfaction;

typedef struct {
    int activity_id;

    char name[MAX_NAME_LENGTH];
    ActivityType category;

    time_t start_time;
    time_t end_time;
    long duration_seconds;

    ActivityClassification classification;
    DistractionReason distraction_reason;

    char context[MAX_CONTEXT_LENGTH];
} Activity;

#endif
