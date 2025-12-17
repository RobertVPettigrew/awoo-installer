/* source/download_manager.c - download manager skeleton */

#include "download_manager.h"
#include <stdio.h>

int download_manager_init(void) {
    // TODO: init queue structures, load persisted queue
    fprintf(stderr, "download_manager_init: stub\n");
    return 0;
}

int download_manager_queue_download(const app_record_t *record, const char *store_id, int auto_install) {
    (void)record; (void)store_id; (void)auto_install;
    // TODO: add record to persistent queue
    fprintf(stderr, "download_manager_queue_download: queued %s\n", record ? record->id : "(null)");
    return 0;
}

int download_manager_start(void) {
    // TODO: process downloads with http_wrapper and call decompress/verify/install
    fprintf(stderr, "download_manager_start: stub\n");
    return 0;
}

void download_manager_shutdown(void) {
    // TODO: cleanup
}
