#include <stdio.h>
#include <string.h>

#include "session.h"
#include "activity.h"
#include "insights.h"
#include "storage.h"
#include "report.h"

static void clear_input_buffer(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* Discard remaining input. */
    }
}

static int read_integer(const char *prompt)
{
    int value;

    while (1) {
        printf("%s", prompt);

        if (scanf("%d", &value) == 1) {
            clear_input_buffer();
            return value;
        }

        printf("Invalid input. Please enter a number.\n");
        clear_input_buffer();
    }
}

static void read_line(
    const char *prompt,
    char *buffer,
    size_t size
)
{
    printf("%s", prompt);

    if (fgets(buffer, (int)size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

static const char *intent_to_string(IntentType intent)
{
    switch (intent) {
        case INTENT_STUDY:
            return "Study";

        case INTENT_WORK:
            return "Work";

        case INTENT_COMMUNICATION:
            return "Communication";

        case INTENT_ENTERTAINMENT:
            return "Entertainment";

        case INTENT_PERSONAL:
            return "Personal Task";

        case INTENT_NO_PURPOSE:
            return "No Specific Purpose";

        default:
            return "Unknown";
    }
}

static const char *device_to_string(DeviceType device)
{
    switch (device) {
        case DEVICE_PHONE:
            return "Phone";

        case DEVICE_LAPTOP:
            return "Laptop";

        case DEVICE_TABLET:
            return "Tablet";

        case DEVICE_OTHER:
            return "Other";

        default:
            return "Unknown";
    }
}

static IntentType select_intention(void)
{
    int choice;

    while (1) {
        printf("\nSelect your intention:\n");
        printf("1. Study\n");
        printf("2. Work\n");
        printf("3. Communication\n");
        printf("4. Entertainment\n");
        printf("5. Personal Task\n");
        printf("6. No Specific Purpose\n");

        choice = read_integer("Enter choice: ");

        if (choice >= INTENT_STUDY &&
            choice <= INTENT_NO_PURPOSE) {
            return (IntentType)choice;
        }

        printf("Invalid intention. Please select 1-6.\n");
    }
}

static DeviceType select_device(void)
{
    int choice;

    while (1) {
        printf("\nSelect device:\n");
        printf("1. Phone\n");
        printf("2. Laptop\n");
        printf("3. Tablet\n");
        printf("4. Other\n");

        choice = read_integer("Enter choice: ");

        if (choice >= DEVICE_PHONE &&
            choice <= DEVICE_OTHER) {
            return (DeviceType)choice;
        }

        printf("Invalid device. Please select 1-4.\n");
    }
}

static ActivityType select_activity_type(void)
{
    int choice;

    while (1) {
        printf("\nSelect activity category:\n");
        printf("1. Study\n");
        printf("2. Work\n");
        printf("3. Communication\n");
        printf("4. Entertainment\n");
        printf("5. Personal Task\n");
        printf("6. Other\n");

        choice = read_integer("Enter choice: ");

        if (choice >= ACTIVITY_STUDY &&
            choice <= ACTIVITY_OTHER) {
            return (ActivityType)choice;
        }

        printf(
            "Invalid activity category. "
            "Please select 1-6.\n"
        );
    }
}

static DistractionReason select_distraction_reason(void)
{
    int choice;

    while (1) {
        printf("\nWhy did this drift happen?\n");
        printf("1. Notification\n");
        printf("2. Boredom\n");
        printf("3. Habit\n");
        printf("4. Curiosity\n");
        printf("5. New Task\n");
        printf("6. Planned Break\n");
        printf("7. Other\n");

        choice = read_integer("Enter choice: ");

        if (choice >= 1 && choice <= 7) {
            return (DistractionReason)choice;
        }

        printf("Invalid reason. Please select 1-7.\n");
    }
}

static void display_active_session(const Session *session)
{
    if (!session->is_active) {
        printf("\nNo active session.\n");
        return;
    }

    printf("\n========== ACTIVE SESSION ==========\n");

    printf(
        "Session ID       : %d\n",
        session->session_id
    );

    printf(
        "Original Intent  : %s\n",
        intent_to_string(session->original_intent)
    );

    printf(
        "Current Intent   : %s\n",
        intent_to_string(session->current_intent)
    );

    printf(
        "Device           : %s\n",
        device_to_string(session->device_type)
    );

    if (session->original_intent != INTENT_NO_PURPOSE) {
        printf(
            "Goal             : %s\n",
            session->goal
        );
    } else {
        printf("Goal             : N/A\n");
    }

    if (session->expected_duration_minutes > 0) {
        printf(
            "Expected Duration: %d minutes\n",
            session->expected_duration_minutes
        );
    } else {
        printf(
            "Expected Duration: Not specified\n"
        );
    }

    printf(
        "Activities       : %d\n",
        session->activity_count
    );

    printf(
        "Aligned          : %d\n",
        session->aligned_count
    );

    printf(
        "Contextual       : %d\n",
        session->contextual_count
    );

    printf(
        "Intent Changes   : %d\n",
        session->intent_change_count
    );

    printf(
        "Drifts           : %d\n",
        session->drift_count
    );

    printf("====================================\n");
}

static void create_new_session(
    Session *session,
    int session_id
)
{
    int expected_minutes;

    if (session->is_active) {
        printf(
            "\nA session is already active. "
            "End it before starting another session.\n"
        );
        return;
    }

    initialize_session(session, session_id);

    session->original_intent = select_intention();
    session->current_intent =
        session->original_intent;

    session->device_type = select_device();

    if (session->original_intent !=
        INTENT_NO_PURPOSE) {
        read_line(
            "Enter your specific goal: ",
            session->goal,
            sizeof(session->goal)
        );
    }

    while (1) {
        expected_minutes =
            read_integer(
                "Expected duration in minutes "
                "(0 if not specified): "
            );

        if (expected_minutes >= 0) {
            break;
        }

        printf(
            "Duration cannot be negative.\n"
        );
    }

    session->expected_duration_minutes =
        expected_minutes;

    if (start_session(session)) {
        printf(
            "\nSession %d started successfully.\n",
            session->session_id
        );

        display_active_session(session);
    } else {
        printf("\nUnable to start session.\n");
    }
}

static void record_activity(Session *session)
{
    Activity activity;
    int relationship_choice = 0;

    if (!session->is_active) {
        printf(
            "\nStart a session before "
            "recording an activity.\n"
        );
        return;
    }

    if (session->activity_count >= MAX_ACTIVITIES) {
        printf(
            "\nMaximum activity limit reached "
            "for this session.\n"
        );
        return;
    }

    initialize_activity(
        &activity,
        session->activity_count + 1
    );

    printf(
        "\n========== RECORD ACTIVITY ==========\n"
    );

    read_line(
        "Activity name: ",
        activity.name,
        sizeof(activity.name)
    );

    activity.category = select_activity_type();

    if (!start_activity(&activity)) {
        printf(
            "\nUnable to start activity tracking.\n"
        );
        return;
    }

    if (session->original_intent ==
        INTENT_NO_PURPOSE) {

        activity.classification =
            CLASS_NO_PURPOSE;

    } else if (
        (int)activity.category ==
        (int)session->current_intent
    ) {

        activity.classification =
            CLASS_ALIGNED;

    } else {

        printf(
            "\nThis activity differs from "
            "your current intent: %s\n",
            intent_to_string(
                session->current_intent
            )
        );

        printf(
            "\nHow is this activity related "
            "to your intention?\n"
        );

        printf(
            "1. Related/contextual activity\n"
        );

        printf(
            "2. Intentional change of intention\n"
        );

        printf(
            "3. Unrelated drift/distraction\n"
        );

        while (
            relationship_choice < 1 ||
            relationship_choice > 3
        ) {
            relationship_choice =
                read_integer("Enter choice: ");

            if (
                relationship_choice < 1 ||
                relationship_choice > 3
            ) {
                printf(
                    "Invalid choice. "
                    "Please select 1-3.\n"
                );
            }
        }

        activity.classification =
            classify_activity(
                session,
                activity.category,
                relationship_choice
            );
    }

    if (
        activity.classification ==
        CLASS_DRIFT
    ) {
        set_distraction_reason(
            &activity,
            select_distraction_reason()
        );
    }

    read_line(
        "Context / short note (optional): ",
        activity.context,
        sizeof(activity.context)
    );

    if (!end_activity(&activity)) {
        printf(
            "\nUnable to finish "
            "activity tracking.\n"
        );
        return;
    }

    if (!add_activity_to_session(
            session,
            &activity
        )) {
        printf(
            "\nUnable to add activity "
            "to the session.\n"
        );
        return;
    }

    printf(
        "\n========== ACTIVITY RECORDED ==========\n"
    );

    printf(
        "Activity        : %s\n",
        activity.name
    );

    printf(
        "Category        : %s\n",
        activity_type_to_string(
            activity.category
        )
    );

    printf(
        "Classification  : %s\n",
        classification_to_string(
            activity.classification
        )
    );

    printf(
        "Duration        : %ld seconds\n",
        activity.duration_seconds
    );

    if (
        activity.classification ==
        CLASS_DRIFT
    ) {
        printf(
            "Drift Reason    : %s\n",
            distraction_reason_to_string(
                activity.distraction_reason
            )
        );
    }

    if (activity.context[0] != '\0') {
        printf(
            "Context         : %s\n",
            activity.context
        );
    }

    printf(
        "Total Activities : %d\n",
        session->activity_count
    );

    printf(
        "=======================================\n"
    );
}

static void finish_current_session(
    Session *session,
    const StorageConfig *storage_config
)
{
    if (!session->is_active) {
        printf(
            "\nNo active session to end.\n"
        );
        return;
    }

    if (!end_session(session)) {
        printf(
            "\nUnable to end session.\n"
        );
        return;
    }

    printf(
        "\n========== SESSION ENDED ==========\n"
    );

    printf(
        "Session ID       : %d\n",
        session->session_id
    );

    printf(
        "Intent           : %s\n",
        intent_to_string(
            session->original_intent
        )
    );

    printf(
        "Final Intent     : %s\n",
        intent_to_string(
            session->current_intent
        )
    );

    printf(
        "Device           : %s\n",
        device_to_string(
            session->device_type
        )
    );

    printf(
        "Duration         : %ld seconds\n",
        session->actual_duration_seconds
    );

    printf(
        "Activities       : %d\n",
        session->activity_count
    );

    printf(
        "Aligned          : %d\n",
        session->aligned_count
    );

    printf(
        "Contextual       : %d\n",
        session->contextual_count
    );

    printf(
        "Intent Changes   : %d\n",
        session->intent_change_count
    );

    printf(
        "Drifts           : %d\n",
        session->drift_count
    );

    if (session->alignment_percentage < 0.0) {
        printf(
            "Intent Alignment : N/A\n"
        );
    } else {
        printf(
            "Intent Alignment : %.2f%%\n",
            session->alignment_percentage
        );
    }

    printf(
        "====================================\n"
    );

    if (storage_config != NULL) {
        StorageStatus storage_status;

        storage_status = save_session(
            storage_config,
            session
        );

        if (storage_status == STORAGE_SUCCESS) {
            printf(
                "Session saved to persistent history.\n"
            );
        } else {
            printf(
                "Warning: session could not be saved: %s\n",
                storage_status_to_string(
                    storage_status
                )
            );
        }
    }

}

static void display_session_history(
    const StorageConfig *storage_config
)
{
    Session sessions[20];
    size_t loaded_count = 0;
    size_t index;
    StorageStatus status;

    if (storage_config == NULL) {
        return;
    }

    status = load_sessions(
        storage_config,
        sessions,
        20,
        &loaded_count
    );

    if (status != STORAGE_SUCCESS) {
        printf(
            "\nUnable to load session history: %s\n",
            storage_status_to_string(status)
        );
        return;
    }

    if (loaded_count == 0) {
        printf(
            "\nNo saved session history is available.\n"
        );
        return;
    }

    printf(
        "\n========== SESSION HISTORY ==========\n"
    );

    for (index = 0; index < loaded_count; index++) {
        const Session *session = &sessions[index];

        printf(
            "\nSession %lu\n",
            (unsigned long)(index + 1)
        );

        printf(
            "Session ID       : %d\n",
            session->session_id
        );

        printf(
            "Intent           : %s\n",
            intent_to_string(
                session->original_intent
            )
        );

        printf(
            "Device           : %s\n",
            device_to_string(
                session->device_type
            )
        );

        if (session->original_intent ==
            INTENT_NO_PURPOSE) {
            printf(
                "Goal             : N/A\n"
            );
        } else {
            printf(
                "Goal             : %s\n",
                session->goal
            );
        }

        printf(
            "Activities       : %d\n",
            session->activity_count
        );

        if (session->original_intent ==
            INTENT_NO_PURPOSE) {
            printf(
                "Intent Alignment : N/A\n"
            );
        } else {
            printf(
                "Intent Alignment : %.2f%%\n",
                session->alignment_percentage
            );
        }

        printf(
            "Duration         : %ld seconds\n",
            session->actual_duration_seconds
        );

        printf(
            "-------------------------------------\n"
        );
    }

    printf(
        "=====================================\n"
    );
}

static void display_behaviour_insights(
    const Session *session
)
{
    BehaviourInsights insights;

    if (session == NULL) {
        return;
    }

    if (session->activity_count == 0) {
        printf(
            "\nNo activity data is available "
            "for behaviour insights.\n"
        );
        return;
    }

    if (!calculate_behaviour_insights(
            session,
            &insights
        )) {
        printf(
            "\nUnable to calculate "
            "behaviour insights.\n"
        );
        return;
    }

    printf(
        "\n========== BEHAVIOUR INSIGHTS ==========\n"
    );

    printf(
        "Total Activities        : %d\n",
        insights.total_activities
    );

    printf(
        "Aligned                 : %d (%.2f%%)\n",
        insights.aligned_activities,
        insights.aligned_percentage
    );

    printf(
        "Contextual              : %d (%.2f%%)\n",
        insights.contextual_activities,
        insights.contextual_percentage
    );

    printf(
        "Intent Changes          : %d (%.2f%%)\n",
        insights.intent_changes,
        insights.intent_change_percentage
    );

    printf(
        "Drifts                  : %d (%.2f%%)\n",
        insights.drift_activities,
        insights.drift_percentage
    );

    printf(
        "Dominant Behaviour      : %s\n",
        dominant_behaviour_to_string(
            &insights
        )
    );

    if (insights.drift_activities > 0) {
        printf(
            "Most Common Drift Reason: %s\n",
            distraction_reason_to_string(
                insights.most_common_distraction_reason
            )
        );
    } else {
        printf(
            "Most Common Drift Reason: None\n"
        );
    }

    if (session->original_intent ==
        INTENT_NO_PURPOSE) {
        printf(
            "Intent Alignment       : N/A\n"
        );
    } else {
        printf(
            "Intent Alignment       : %.2f%%\n",
            calculate_alignment_percentage(
                session
            )
        );
    }

    printf(
        "========================================\n"
    );
}

static void display_historical_report(
    const StorageConfig *storage_config
)
{
    Session sessions[20];
    size_t loaded_count = 0;
    StorageStatus storage_status;
    HistoricalReport report;

    if (storage_config == NULL) {
        return;
    }

    storage_status = load_sessions(
        storage_config,
        sessions,
        20,
        &loaded_count
    );

    if (storage_status != STORAGE_SUCCESS) {
        printf(
            "\nUnable to load historical report: %s\n",
            storage_status_to_string(
                storage_status
            )
        );
        return;
    }

    if (loaded_count == 0) {
        printf(
            "\nNo saved sessions are available "
            "for historical reporting.\n"
        );
        return;
    }

    if (!generate_historical_report(
            sessions,
            loaded_count,
            &report
        )) {
        printf(
            "\nUnable to generate historical report.\n"
        );
        return;
    }

    printf(
        "\n========================================\n"
        "       HISTORICAL BEHAVIOUR REPORT\n"
        "========================================\n"
    );

    printf(
        "Total Sessions          : %lu\n",
        (unsigned long)report.total_sessions
    );

    printf(
        "Total Activities        : %d\n",
        report.total_activities
    );

    printf(
        "Total Tracked Time      : %ld seconds\n",
        report.total_duration_seconds
    );

    printf(
        "Average Session Duration: %.2f seconds\n",
        report.average_duration_seconds
    );

    printf(
        "Aligned Activities      : %d\n",
        report.total_aligned
    );

    printf(
        "Contextual Activities   : %d\n",
        report.total_contextual
    );

    printf(
        "Intent Changes          : %d\n",
        report.total_intent_changes
    );

    printf(
        "Drift Activities        : %d\n",
        report.total_drifts
    );

    printf(
        "Average Alignment       : %.2f%%\n",
        report.average_alignment_percentage
    );

    printf(
        "Highest Alignment       : %.2f%%\n",
        report.highest_alignment_percentage
    );

    printf(
        "Lowest Alignment        : %.2f%%\n",
        report.lowest_alignment_percentage
    );

    printf(
        "Sessions With Goals     : %d\n",
        report.sessions_with_goals
    );

    printf(
        "Completed Goals         : %d\n",
        report.completed_goals
    );

    printf(
        "Goal Completion Rate    : %.2f%%\n",
        calculate_goal_completion_percentage(
            &report
        )
    );

    printf(
        "Most Common Intent      : %s\n",
        intent_to_string(
            report.most_common_intent
        )
    );

    if (report.most_common_distraction_reason ==
        REASON_NONE) {
        printf(
            "Most Common Distraction : None\n"
        );
    } else {
        printf(
            "Most Common Distraction : %s\n",
            distraction_reason_to_string(
                report.most_common_distraction_reason
            )
        );
    }

    printf(
        "Dominant Pattern        : %s\n",
        report_dominant_pattern(
            &report
        )
    );

    printf(
        "========================================\n"
    );
}

static void display_main_menu(void)
{
    printf(
        "\n========================================\n"
    );

    printf(
        "                 WHY NOW\n"
    );

    printf(
        "      Digital Behaviour Awareness\n"
    );

    printf(
        "========================================\n"
    );

    printf("1. Start New Session\n");
    printf("2. View Active Session\n");
    printf("3. Record Activity\n");
    printf("4. End Current Session\n");
    printf("5. View Session History\n");
    printf("6. View Behaviour Insights\n");
    printf("7. View Historical Report\n");
    printf("8. About & Privacy\n");
    printf("0. Exit\n");
}

int main(void)
{
    Session current_session;
    StorageConfig storage_config;
    StorageStatus storage_status;
    Session saved_sessions[20];
    size_t saved_session_count = 0;
    int next_session_id = 1;
    int choice;
    int running = 1;

    initialize_storage_config(
        &storage_config
    );

    storage_status = initialize_storage(
        &storage_config
    );

    if (storage_status != STORAGE_SUCCESS) {
        printf(
            "Unable to initialize persistent storage: %s\n",
            storage_status_to_string(
                storage_status
            )
        );
        return 1;
    }

    storage_status = load_sessions(
        &storage_config,
        saved_sessions,
        20,
        &saved_session_count
    );

    if (storage_status == STORAGE_SUCCESS &&
        saved_session_count > 0) {
        next_session_id =
            saved_sessions[
                saved_session_count - 1
            ].session_id + 1;
    }

    initialize_session(
        &current_session,
        next_session_id
    );

    while (running) {
        display_main_menu();

        choice =
            read_integer("Enter choice: ");

        switch (choice) {
            case 1:
                if (!current_session.is_active) {
                    create_new_session(
                        &current_session,
                        next_session_id
                    );

                    if (
                        current_session.is_active
                    ) {
                        next_session_id++;
                    }
                } else {
                    printf(
                        "\nA session is already active.\n"
                    );
                }

                break;

            case 2:
                display_active_session(
                    &current_session
                );
                break;

            case 3:
                record_activity(
                    &current_session
                );
                break;

            case 4:
                finish_current_session(
                    &current_session,
                    &storage_config
                );
                break;

            case 5:
                display_session_history(
                    &storage_config
                );
                break;

            case 6:
                display_behaviour_insights(
                    &current_session
                );
                break;

            case 7:
                display_historical_report(
                    &storage_config
                );
                break;

            case 8:
                printf(
                    "\nWhyNow records user-provided "
                    "session information for digital "
                    "behaviour awareness.\n"
                );

                printf(
                    "The current prototype stores no "
                    "passwords, messages, or "
                    "browser history.\n"
                );

                break;

            case 0:
                if (current_session.is_active) {
                    printf(
                        "\nPlease end the active "
                        "session before exiting.\n"
                    );
                } else {
                    printf(
                        "\nThank you for using WhyNow.\n"
                    );

                    running = 0;
                }

                break;

            default:
                printf(
                    "\nInvalid menu choice. "
                    "Please select 0-8.\n"
                );
        }
    }

    return 0;
}
