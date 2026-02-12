#ifndef __UTILITY_ASSET_LOADER_H__
#define __UTILITY_ASSET_LOADER_H__

#include <stddef.h>
#include <stdint.h>

uint8_t *loadAssetBinary(const char *filename);
void freeAssetBinary(uint8_t *data);
size_t assetGetSize(uint8_t *data);

#endif
