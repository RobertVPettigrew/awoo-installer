/* source/http_wrapper.c - simple http wrapper (stub)
 * Uses libnx/httpc in a real implementation. This is a skeleton with TODOs.
 */

#include "http_wrapper.h"
#include <stdio.h>

int http_wrapper_init(void) {
    // TODO: call httpcInit() and related libnx setup
    return 0; // success
}

int http_wrapper_get(const char *url, const char *headers[], int headers_count, void (*data_cb)(const void*, size_t, void*)) {
    // TODO: implement HTTPS GET using httpc; call data_cb for each chunk received
    (void)url; (void)headers; (void)headers_count; (void)data_cb;
    fprintf(stderr, "http_wrapper_get: stub called for url=%s\n", url ? url : "(null)");
    return -1; // not implemented
}

int http_wrapper_get_range(const char *url, unsigned long long start, const char *headers[], int headers_count, void (*data_cb)(const void*, size_t, void*), void *userdata) {
    (void)start; (void)headers; (void)headers_count; (void)data_cb; (void)userdata;
    fprintf(stderr, "http_wrapper_get_range: stub called for url=%s start=%llu\n", url ? url : "(null)", start);
    return -1;
}

void http_wrapper_exit(void) {
    // TODO: cleanup httpc
}
