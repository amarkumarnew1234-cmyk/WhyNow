#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "session.h"
#include "storage.h"

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

static void check_test(
    int condition,
    const char *description
)
{
    tests_run++;

    if (condition) {
        tests_passed++;
        printf("[PASS] %s\n", description);
    } else {
        tests_failed++;
        printf("[FAIL] %s\n", description);
    }
}

int main(void)
{
    const char *test_file =
        "test_sessions_storage.dat";

    StorageConfig config;
    StorageConfig invalid_config;

    Session first_session;
    Session second_session;
    Session loaded_sessions[4];
    Session limited_sessions[1];

    size_t loaded_count = 0;
    size_t limited_count = 0;

    StorageStatus status;

    printf(
        "====================================\n"
        "       WhyNow Storage Tests\n"
        "====================================\n\n"
    );

    remove(test_file);

    initialize_storage_config(&config);

    check_test(
        config.file_path != NULL,
        "storage config initializes a file path"
    );

    check_test(
        strcmp(
            config.file_path,
            DEFAULT_SESSION_DATA_FILE
        ) == 0,
        "storage config uses default session file"
    );

    config.file_path = test_file;

    check_test(
        storage_file_exists(&config) == 0,
        "storage file does not exist initially"
    );

    status = initialize_storage(&config);

    check_test(
        status == STORAGE_SUCCESS,
        "storage initializes successfully"
    );

    check_test(
        storage_file_exists(&config) == 1,
        "storage file exists after initialization"
    );

    initialize_session(&first_session, 101);
    first_session.original_intent = INTENT_STUDY;
    first_session.current_intent = INTENT_STUDY;
    first_session.device_type = DEVICE_LAPTOP;

    strncpy(
        first_session.goal,
        "Prepare DAA",
        sizeof(first_session.goal) - 1
    );
    first_session.goal[
        sizeof(first_session.goal) - 1
    ] = '\0';

    first_session.expected_duration_minutes = 60;

    status = save_session(
        &config,
        &first_session
    );

    check_test(
        status == STORAGE_SUCCESS,
        "first session saves successfully"
    );

    initialize_session(&second_session, 102);
    second_session.original_intent = INTENT_WORK;
    second_session.current_intent = INTENT_WORK;
    second_session.device_type = DEVICE_LAPTOP;

    strncpy(
        second_session.goal,
        "Complete DevOps project",
        sizeof(second_session.goal) - 1
    );
    second_session.goal[
        sizeof(second_session.goal) - 1
    ] = '\0';

    second_session.expected_duration_minutes = 90;

    status = save_session(
        &config,
        &second_session
    );

    check_test(
        status == STORAGE_SUCCESS,
        "second session saves successfully"
    );

    status = load_sessions(
        &config,
        loaded_sessions,
        4,
        &loaded_count
    );

    check_test(
        status == STORAGE_SUCCESS,
        "stored sessions load successfully"
    );

    check_test(
        loaded_count == 2,
        "two stored sessions are loaded"
    );

    check_test(
        loaded_sessions[0].session_id == 101,
        "first session ID is preserved"
    );

    check_test(
        strcmp(
            loaded_sessions[0].goal,
            "Prepare DAA"
        ) == 0,
        "first session goal is preserved"
    );

    check_test(
        loaded_sessions[1].session_id == 102,
        "second session ID is preserved"
    );

    check_test(
        strcmp(
            loaded_sessions[1].goal,
            "Complete DevOps project"
        ) == 0,
        "second session goal is preserved"
    );

    status = load_sessions(
        &config,
        limited_sessions,
        1,
        &limited_count
    );

    check_test(
        status == STORAGE_ERROR_CAPACITY,
        "insufficient load capacity is detected"
    );

    check_test(
        save_session(
            NULL,
            &first_session
        ) == STORAGE_ERROR_INVALID_ARGUMENT,
        "save rejects null storage config"
    );

    check_test(
        save_session(
            &config,
            NULL
        ) == STORAGE_ERROR_INVALID_ARGUMENT,
        "save rejects null session"
    );

    check_test(
        load_sessions(
            &config,
            loaded_sessions,
            0,
            &loaded_count
        ) == STORAGE_ERROR_INVALID_ARGUMENT,
        "load rejects zero capacity"
    );

    invalid_config.file_path = NULL;

    check_test(
        initialize_storage(
            &invalid_config
        ) == STORAGE_ERROR_INVALID_ARGUMENT,
        "storage initialization rejects null file path"
    );

    check_test(
        strcmp(
            storage_status_to_string(
                STORAGE_SUCCESS
            ),
            "Success"
        ) == 0,
        "storage success status has readable text"
    );

    remove(test_file);

    printf(
        "\n====================================\n"
    );

    printf(
        "Tests run   : %d\n",
        tests_run
    );

    printf(
        "Tests passed: %d\n",
        tests_passed
    );

    printf(
        "Tests failed: %d\n",
        tests_failed
    );

    printf(
        "====================================\n"
    );

    if (tests_failed == 0) {
        printf(
            "RESULT: ALL STORAGE TESTS PASSED\n"
        );
        return 0;
    }

    printf(
        "RESULT: STORAGE TESTS FAILED\n"
    );

    return 1;
}
