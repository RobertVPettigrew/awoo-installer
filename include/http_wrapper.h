/* include/http_wrapper.h - simple http wrapper (stub) */
#ifndef HTTP_WRAPPER_H
#define HTTP_WRAPPER_H

#include <stddef.h>

int http_wrapper_init(void);
int http_wrapper_get(const char *url, const char *headers[], int headers_count, void (*data_cb)(const void*, size_t, void*), void *userdata);
int http_wrapper_get_range(const char *url, unsigned long long start, const char *headers[], int headers_count, void (*data_cb)(const void*, size_t, void*), void *userdata);
void http_wrapper_exit(void);

#endif // HTTP_WRAPPER_H
