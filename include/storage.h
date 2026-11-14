#ifndef STORAGE_H
#define STORAGE_H

#include <stddef.h>

#include "session.h"

#define DEFAULT_SESSION_DATA_FILE "data/sessions.dat"

typedef enum {
    STORAGE_SUCCESS = 0,
    STORAGE_ERROR_INVALID_ARGUMENT,
    STORAGE_ERROR_OPEN_FILE,
    STORAGE_ERROR_WRITE,
    STORAGE_ERROR_READ,
    STORAGE_ERROR_CAPACITY
} StorageStatus;

typedef struct {
    const char *file_path;
} StorageConfig;

void initialize_storage_config(StorageConfig *config);

int storage_file_exists(const StorageConfig *config);

StorageStatus initialize_storage(
    const StorageConfig *config
);

StorageStatus save_session(
    const StorageConfig *config,
    const Session *session
);

const char *storage_status_to_string(
    StorageStatus status
);

#endif