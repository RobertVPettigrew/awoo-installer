/* include/manifest_parser.h - parse store manifest (stub) */
#ifndef MANIFEST_PARSER_H
#define MANIFEST_PARSER_H

#include <stddef.h>

#define MAX_TAGS 8

typedef struct {
    char id[128];
    char name[128];
    char short_description[256];
    char url[512];
    char icon_url[512];
    char sha256[128];
    unsigned long long size_bytes;
    unsigned long long uncompressed_size;
    char type[16]; // nsp, xci, ncz, etc.
} app_record_t;

// Parse a manifest JSON buffer. Returns number of records parsed (0 on failure).
size_t parse_manifest(const char *buf, app_record_t *out_records, size_t max_records);

#endif // MANIFEST_PARSER_H
