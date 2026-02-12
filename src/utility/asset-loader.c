#include "utility/asset-loader.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AssetBinaryHeader {
	size_t size;
} AssetBinaryHeader;

uint8_t *loadAssetBinary(const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "Error opening asset '%s'\n", filename);
		return 0;
	}
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);

	const size_t nullTerminationSize = 1; // For the null terminator
	AssetBinaryHeader *asset = malloc(sizeof(AssetBinaryHeader) + size + nullTerminationSize);
	asset->size = size;
	uint8_t *string = (uint8_t*)(((uintptr_t)asset) + sizeof(AssetBinaryHeader));
	fread(string, size, 1, file);
	fclose(file);

	// Null-terminate the string
	string[size] = 0;

	return string;
}

void freeAssetBinary(uint8_t *data) {
	if (!data) return;

	AssetBinaryHeader *asset = (AssetBinaryHeader*)(((uintptr_t)data) - sizeof(AssetBinaryHeader));
	memset(asset, 0, asset->size + sizeof(AssetBinaryHeader));
	free(asset);
}

size_t assetGetSize(uint8_t *data) {
	if (!data) return 0;
	return ((AssetBinaryHeader*)(((uintptr_t)data) - sizeof(AssetBinaryHeader)))->size;
}
