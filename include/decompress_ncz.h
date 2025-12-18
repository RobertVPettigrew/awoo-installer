/* include/decompress_ncz.h - NCZ decompression stub API */
#ifndef DECOMPRESS_NCZ_H
#define DECOMPRESS_NCZ_H

#include <stddef.h>

// Decompress a compressed NCZ file at src_path to dest_path.
// Returns 0 on success, non-zero on failure.
int decompress_ncz_file(const char *src_path, const char *dest_path, void (*progress_cb)(size_t, size_t, void*), void *userdata);

#endif // DECOMPRESS_NCZ_H
