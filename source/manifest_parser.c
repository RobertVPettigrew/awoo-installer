/* source/manifest_parser.c - implement manifest parsing using jsmn */

#include "manifest_parser.h"
#include "jsmn.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    return (int)(tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start
        && strncmp(json + tok->start, s, tok->end - tok->start) == 0);
}

static void token_to_str(const char *json, jsmntok_t *tok, char *out, size_t outlen) {
    size_t len = (size_t)(tok->end - tok->start);
    if (len >= outlen) len = outlen - 1;
    memcpy(out, json + tok->start, len);
    out[len] = '\0';
}

size_t parse_manifest(const char *buf, app_record_t *out_records, size_t max_records) {
    if (!buf || !out_records || max_records == 0) return 0;
    jsmn_parser p;
    jsmntok_t tokens[1024];
    jsmn_init(&p);
    int tokn = jsmn_parse(&p, buf, strlen(buf), tokens, sizeof(tokens)/sizeof(tokens[0]));
    if (tokn < 0) {
        fprintf(stderr, "parse_manifest: JSON parse error\n");
        return 0;
    }

    size_t out_idx = 0;
    // find "apps" array
    for (int i = 0; i < tokn; i++) {
        if (tokens[i].type == JSMN_STRING && jsoneq(buf, &tokens[i], "apps")) {
            // the next token should be array
            if (i + 1 < tokn && tokens[i+1].type == JSMN_ARRAY) {
                int arr_idx = i + 1;
                int j = arr_idx + 1;
                // iterate elements in array
                while (j < tokn && out_idx < max_records) {
                    if (tokens[j].type == JSMN_OBJECT) {
                        // parse object fields
                        int obj_start = j;
                        int obj_end = tokens[j].end;
                        // iterate over key/value pairs inside object
                        int k = j + 1;
                        app_record_t rec;
                        memset(&rec, 0, sizeof(rec));
                        while (k < tokn && tokens[k].start < obj_end) {
                            if (tokens[k].type == JSMN_STRING) {
                                jsmntok_t *key = &tokens[k];
                                jsmntok_t *val = &tokens[k+1];
                                if (jsoneq(buf, key, "id")) {
                                    token_to_str(buf, val, rec.id, sizeof(rec.id));
                                } else if (jsoneq(buf, key, "name")) {
                                    token_to_str(buf, val, rec.name, sizeof(rec.name));
                                } else if (jsoneq(buf, key, "url")) {
                                    token_to_str(buf, val, rec.url, sizeof(rec.url));
                                } else if (jsoneq(buf, key, "type")) {
                                    token_to_str(buf, val, rec.type, sizeof(rec.type));
                                } else if (jsoneq(buf, key, "sha256")) {
                                    token_to_str(buf, val, rec.sha256, sizeof(rec.sha256));
                                } else if (jsoneq(buf, key, "size_bytes")) {
                                    char tmp[64];
                                    token_to_str(buf, val, tmp, sizeof(tmp));
                                    rec.size_bytes = strtoull(tmp, NULL, 10);
                                }
                                k += 2;
                            } else {
                                k++;
                            }
                        }
                        out_records[out_idx++] = rec;
                        j = obj_end;
                    } else {
                        j++;
                    }
                }
                break;
            }
        }
    }
    return out_idx;
}
