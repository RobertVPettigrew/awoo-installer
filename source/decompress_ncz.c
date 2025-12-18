/* source/decompress_ncz.c - stub for NCZ decompression
 * TODO: implement using appropriate decompression library (NSZ/ZSTD/LZ4/XZ) once algorithm confirmed.
 */

#include "decompress_ncz.h"
#include <stdio.h>

int decompress_ncz_file(const char *src_path, const char *dest_path, void (*progress_cb)(size_t, size_t, void*), void *userdata) {
    (void)src_path; (void)dest_path; (void)progress_cb; (void)userdata;
    fprintf(stderr, "decompress_ncz_file: stub called src=%s dest=%s\n", src_path ? src_path : "(null)", dest_path ? dest_path : "(null)");
    return -1; // not implemented
}
