/* include/download_manager.h - download manager API (stub) */
#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H

#include <stddef.h>
#include "manifest_parser.h"

int download_manager_init(void);
int download_manager_queue_download(const app_record_t *record, const char *store_id, int auto_install);
int download_manager_start(void); // start processing queue
void download_manager_shutdown(void);

#endif // DOWNLOAD_MANAGER_H
