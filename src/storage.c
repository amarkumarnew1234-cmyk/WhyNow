#include <stdio.h>

#include "storage.h"

void initialize_storage_config(StorageConfig *config)
{
    if (config == NULL) {
        return;
    }

    config->file_path = DEFAULT_SESSION_DATA_FILE;
}

int storage_file_exists(const StorageConfig *config)
{
    FILE *file;

    if (config == NULL || config->file_path == NULL) {
        return 0;
    }

    file = fopen(config->file_path, "rb");

    if (file == NULL) {
        return 0;
    }

    fclose(file);

    return 1;
}

StorageStatus initialize_storage(
    const StorageConfig *config
)
{
    FILE *file;

    if (config == NULL || config->file_path == NULL) {
        return STORAGE_ERROR_INVALID_ARGUMENT;
    }

    if (storage_file_exists(config)) {
        return STORAGE_SUCCESS;
    }

    file = fopen(config->file_path, "wb");

    if (file == NULL) {
        return STORAGE_ERROR_OPEN_FILE;
    }

    if (fclose(file) != 0) {
        return STORAGE_ERROR_WRITE;
    }

    return STORAGE_SUCCESS;
}

StorageStatus save_session(
    const StorageConfig *config,
    const Session *session
)
{
    FILE *file;
    size_t written;

    if (config == NULL ||
        config->file_path == NULL ||
        session == NULL) {
        return STORAGE_ERROR_INVALID_ARGUMENT;
    }

    file = fopen(config->file_path, "ab");

    if (file == NULL) {
        return STORAGE_ERROR_OPEN_FILE;
    }

    written = fwrite(
        session,
        sizeof(Session),
        1,
        file
    );

    if (written != 1) {
        fclose(file);
        return STORAGE_ERROR_WRITE;
    }

    if (fclose(file) != 0) {
        return STORAGE_ERROR_WRITE;
    }

    return STORAGE_SUCCESS;
}

StorageStatus load_sessions(
    const StorageConfig *config,
    Session *sessions,
    size_t capacity,
    size_t *loaded_count
)
{
    FILE *file;
    size_t count;

    if (config == NULL ||
        config->file_path == NULL ||
        sessions == NULL ||
        loaded_count == NULL ||
        capacity == 0) {
        return STORAGE_ERROR_INVALID_ARGUMENT;
    }

    *loaded_count = 0;

    file = fopen(config->file_path, "rb");

    if (file == NULL) {
        return STORAGE_ERROR_OPEN_FILE;
    }

    count = fread(
        sessions,
        sizeof(Session),
        capacity,
        file
    );

    if (ferror(file)) {
        fclose(file);
        return STORAGE_ERROR_READ;
    }

    if (!feof(file)) {
        fclose(file);
        return STORAGE_ERROR_CAPACITY;
    }

    if (fclose(file) != 0) {
        return STORAGE_ERROR_READ;
    }

    *loaded_count = count;

    return STORAGE_SUCCESS;
}

const char *storage_status_to_string(
    StorageStatus status
)
{
    switch (status) {
        case STORAGE_SUCCESS:
            return "Success";

        case STORAGE_ERROR_INVALID_ARGUMENT:
            return "Invalid argument";

        case STORAGE_ERROR_OPEN_FILE:
            return "Unable to open storage file";

        case STORAGE_ERROR_WRITE:
            return "Storage write error";

        case STORAGE_ERROR_READ:
            return "Storage read error";

        case STORAGE_ERROR_CAPACITY:
            return "Storage capacity reached";

        default:
            return "Unknown storage error";
    }
}
