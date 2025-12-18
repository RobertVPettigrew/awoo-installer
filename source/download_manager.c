/* source/download_manager.c - simple download manager with resume and SHA256 verification */

#include "download_manager.h"
#include "http_wrapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mbedtls/sha256.h>

#define MAX_QUEUE 16

typedef struct {
    app_record_t record;
    char tmp_path[512];
    int auto_install;
} dl_item_t;

static dl_item_t queue[MAX_QUEUE];
static int q_head = 0, q_tail = 0;

int download_manager_init(void) {
    http_wrapper_init();
    q_head = q_tail = 0;
    return 0;
}

int download_manager_queue_download(const app_record_t *record, const char *store_id, int auto_install) {
    (void)store_id;
    if (!record) return -1;
    int next = q_tail % MAX_QUEUE;
    queue[next].record = *record;
    snprintf(queue[next].tmp_path, sizeof(queue[next].tmp_path), "/tmp/%s.part", record->id);
    queue[next].auto_install = auto_install;
    q_tail++;
    return 0;
}

struct file_writer_ctx { FILE *f; mbedtls_sha256_context sha_ctx; };

static void download_data_cb(const void *buf, size_t len, void *userdata) {
    struct file_writer_ctx *ctx = (struct file_writer_ctx*)userdata;
    if (!ctx || !ctx->f) return;
    fwrite(buf, 1, len, ctx->f);
    mbedtls_sha256_update_ret(&ctx->sha_ctx, (const unsigned char*)buf, len);
}

int download_manager_start(void) {
    while (q_head < q_tail) {
        dl_item_t *it = &queue[q_head % MAX_QUEUE];
        const char *url = it->record.url;
        const char *tmp = it->tmp_path;
        FILE *f = fopen(tmp, "ab+");
        if (!f) {
            fprintf(stderr, "download: failed to open %s\n", tmp);
            q_head++;
            continue;
        }
        // determine current size
        fseek(f, 0, SEEK_END);
        unsigned long long cur = (unsigned long long)ftell(f);
        mbedtls_sha256_init(&((struct file_writer_ctx){0}).sha_ctx); // avoid compiler warnings
        struct file_writer_ctx ctx;
        ctx.f = f;
        mbedtls_sha256_init(&ctx.sha_ctx);
        mbedtls_sha256_starts_ret(&ctx.sha_ctx, 0);
        int rc = 0;
        if (cur > 0) {
            // we need to compute sha of existing data to continue hash - for simplicity, we'll re-read file
            unsigned char buffer[8192];
            FILE *rf = fopen(tmp, "rb");
            if (rf) {
                size_t r;
                while ((r = fread(buffer, 1, sizeof(buffer), rf)) > 0) {
                    mbedtls_sha256_update_ret(&ctx.sha_ctx, buffer, r);
                }
                fclose(rf);
            }
            rc = http_wrapper_get_range(url, cur, NULL, 0, download_data_cb, &ctx);
        } else {
            rc = http_wrapper_get(url, NULL, 0, download_data_cb, &ctx);
        }
        fclose(f);
        unsigned char sha_out[32];
        mbedtls_sha256_finish_ret(&ctx.sha_ctx, sha_out);
        char shahex[65];
        for (int i = 0; i < 32; i++) sprintf(shahex + i*2, "%02x", sha_out[i]);
        shahex[64] = '\0';
        if (it->record.sha256[0] != '\0') {
            if (strcasecmp(shahex, it->record.sha256) != 0) {
                fprintf(stderr, "download_manager: sha mismatch for %s (got %s expected %s)\n", it->record.id, shahex, it->record.sha256);
                q_head++;
                continue;
            }
        }
        // if type == ncz call decompressor (stub)
        if (strcmp(it->record.type, "ncz") == 0) {
            char dest[512];
            snprintf(dest, sizeof(dest), "/tmp/%s.decompressed", it->record.id);
            decompress_ncz_file(tmp, dest, NULL, NULL);
        }
        // TODO: trigger install if auto_install
        q_head++;
    }
    return 0;
}

void download_manager_shutdown(void) {
    http_wrapper_exit();
}
