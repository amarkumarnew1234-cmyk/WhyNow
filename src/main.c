#include <stdio.h>
#include <string.h>

#include "session.h"

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

static void read_line(const char *prompt, char *buffer, size_t size)
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

static void display_active_session(const Session *session)
{
    if (!session->is_active) {
        printf("\nNo active session.\n");
        return;
    }

    printf("\n========== ACTIVE SESSION ==========\n");
    printf("Session ID       : %d\n", session->session_id);
    printf("Original Intent  : %s\n",
           intent_to_string(session->original_intent));
    printf("Current Intent   : %s\n",
           intent_to_string(session->current_intent));
    printf("Device           : %s\n",
           device_to_string(session->device_type));

    if (session->original_intent != INTENT_NO_PURPOSE) {
        printf("Goal             : %s\n", session->goal);
    } else {
        printf("Goal             : N/A\n");
    }

    if (session->expected_duration_minutes > 0) {
        printf("Expected Duration: %d minutes\n",
               session->expected_duration_minutes);
    } else {
        printf("Expected Duration: Not specified\n");
    }

    printf("Activities       : %d\n", session->activity_count);
    printf("====================================\n");
}

static void create_new_session(Session *session, int session_id)
{
    int expected_minutes;

    if (session->is_active) {
        printf("\nA session is already active. "
               "End it before starting another session.\n");
        return;
    }

    initialize_session(session, session_id);

    session->original_intent = select_intention();
    session->current_intent = session->original_intent;

    session->device_type = select_device();

    if (session->original_intent != INTENT_NO_PURPOSE) {
        read_line("Enter your specific goal: ",
                  session->goal,
                  sizeof(session->goal));
    }

    while (1) {
        expected_minutes =
            read_integer("Expected duration in minutes "
                         "(0 if not specified): ");

        if (expected_minutes >= 0) {
            break;
        }

        printf("Duration cannot be negative.\n");
    }

    session->expected_duration_minutes = expected_minutes;

    if (start_session(session)) {
        printf("\nSession %d started successfully.\n",
               session->session_id);
        display_active_session(session);
    } else {
        printf("\nUnable to start session.\n");
    }
}

static void finish_current_session(Session *session)
{
    if (!session->is_active) {
        printf("\nNo active session to end.\n");
        return;
    }

    if (!end_session(session)) {
        printf("\nUnable to end session.\n");
        return;
    }

    printf("\n========== SESSION ENDED ==========\n");
    printf("Session ID      : %d\n", session->session_id);
    printf("Intent          : %s\n",
           intent_to_string(session->original_intent));
    printf("Device          : %s\n",
           device_to_string(session->device_type));
    printf("Duration        : %ld seconds\n",
           session->actual_duration_seconds);

    if (session->alignment_percentage < 0.0) {
        printf("Intent Alignment: N/A\n");
    } else {
        printf("Intent Alignment: %.2f%%\n",
               session->alignment_percentage);
    }

    printf("===================================\n");
}

static void display_main_menu(void)
{
    printf("\n========================================\n");
    printf("                 WHY NOW\n");
    printf("      Digital Behaviour Awareness\n");
    printf("========================================\n");
    printf("1. Start New Session\n");
    printf("2. View Active Session\n");
    printf("3. Record Activity\n");
    printf("4. End Current Session\n");
    printf("5. View Session History\n");
    printf("6. View Behaviour Insights\n");
    printf("7. About & Privacy\n");
    printf("0. Exit\n");
}

int main(void)
{
    Session current_session;
    int next_session_id = 1;
    int choice;
    int running = 1;

    initialize_session(&current_session, next_session_id);

    while (running) {
        display_main_menu();
        choice = read_integer("Enter choice: ");

        switch (choice) {
            case 1:
                if (!current_session.is_active) {
                    create_new_session(&current_session,
                                       next_session_id);

                    if (current_session.is_active) {
                        next_session_id++;
                    }
                } else {
                    printf("\nA session is already active.\n");
                }
                break;

            case 2:
                display_active_session(&current_session);
                break;

            case 3:
                printf("\nActivity recording will be added "
                       "through the activity module.\n");
                break;

            case 4:
                finish_current_session(&current_session);
                break;

            case 5:
                printf("\nSession history module is not "
                       "integrated yet.\n");
                break;

            case 6:
                printf("\nBehaviour insights module is not "
                       "integrated yet.\n");
                break;

            case 7:
                printf("\nWhyNow records user-provided session "
                       "information for digital behaviour "
                       "awareness.\n");
                printf("The current prototype stores no "
                       "passwords, messages, or browser history.\n");
                break;

            case 0:
                if (current_session.is_active) {
                    printf("\nPlease end the active session "
                           "before exiting.\n");
                } else {
                    printf("\nThank you for using WhyNow.\n");
                    running = 0;
                }
                break;

            default:
                printf("\nInvalid menu choice. Please select 0-7.\n");
        }
    }

    return 0;
}
