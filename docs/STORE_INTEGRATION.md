# Storefront integration notes (skeleton)

This branch adds a storefront skeleton: HTTP wrapper, manifest parser, download manager, NCZ decompressor stub, and a UI stub.

Next steps to integrate and build:
- Add cJSON (or jsmn) to parse manifests and implement parse_manifest in source/manifest_parser.c.
- Implement libnx httpc usage inside source/http_wrapper.c (http_wrapper_get and http_wrapper_get_range).
- Implement download queue, resume support, and SHA256 verification in source/download_manager.c.
- Implement NCZ decompression in source/decompress_ncz.c once algorithm is known (NSZ/ZSTD/LZ4/XZ).
- Update Makefile to compile the new sources and include romfs/default_stores.json in romfs packaging.

Paths used by the skeleton:
- Config: SD:/switch/awoo-installer/config/stores.json
- Cache: SD:/switch/awoo-installer/cache/
- Downloads: SD:/switch/awoo-installer/downloads/


End of file list.
