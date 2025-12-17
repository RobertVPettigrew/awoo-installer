/* source/manifest_parser.c - manifest parser stub using cJSON (add cJSON when integrating)
 */

#include "manifest_parser.h"
#include <string.h>
#include <stdio.h>

size_t parse_manifest(const char *buf, app_record_t *out_records, size_t max_records) {
    (void)buf; (void)out_records; (void)max_records;
    // TODO: implement JSON parsing using cJSON or jsmn
    fprintf(stderr, "parse_manifest: stub called\n");
    return 0;
}
