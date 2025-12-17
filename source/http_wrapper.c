/* source/http_wrapper.c - libcurl based http wrapper implementation */

#include "http_wrapper.h"
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct curl_userdata {
    void (*data_cb)(const void*, size_t, void*);
    void *userdata;
};

static size_t curl_write_cb(void *ptr, size_t size, size_t nmemb, void *userdata) {
    struct curl_userdata *cu = (struct curl_userdata*)userdata;
    size_t total = size * nmemb;
    if (cu && cu->data_cb) cu->data_cb(ptr, total, cu->userdata);
    return total;
}

int http_wrapper_init(void) {
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) return -1;
    return 0;
}

int http_wrapper_get(const char *url, const char *headers[], int headers_count, void (*data_cb)(const void*, size_t, void*), void *userdata) {
    if (!url || !data_cb) return -1;
    CURL *curl = curl_easy_init();
    if (!curl) return -1;
    struct curl_slist *hdrs = NULL;
    for (int i = 0; i < headers_count; i++) {
        if (headers[i]) hdrs = curl_slist_append(hdrs, headers[i]);
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);
    struct curl_userdata cu;
    cu.data_cb = data_cb;
    cu.userdata = userdata;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &cu);
    CURLcode rc = curl_easy_perform(curl);
    long code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    curl_slist_free_all(hdrs);
    curl_easy_cleanup(curl);
    return rc == CURLE_OK ? (int)code : -1;
}

int http_wrapper_get_range(const char *url, unsigned long long start, const char *headers[], int headers_count, void (*data_cb)(const void*, size_t, void*), void *userdata) {
    if (!url || !data_cb) return -1;
    CURL *curl = curl_easy_init();
    if (!curl) return -1;
    struct curl_slist *hdrs = NULL;
    for (int i = 0; i < headers_count; i++) {
        if (headers[i]) hdrs = curl_slist_append(hdrs, headers[i]);
    }
    char range_header[64];
    snprintf(range_header, sizeof(range_header), "%llu-", start);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_RANGE, range_header);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);
    struct curl_userdata cu;
    cu.data_cb = data_cb;
    cu.userdata = userdata;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &cu);
    CURLcode rc = curl_easy_perform(curl);
    long code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    curl_slist_free_all(hdrs);
    curl_easy_cleanup(curl);
    return rc == CURLE_OK ? (int)code : -1;
}

void http_wrapper_exit(void) {
    curl_global_cleanup();
}
