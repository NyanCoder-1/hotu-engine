#ifndef __RENDER_TEXTURE_H__
#define __RENDER_TEXTURE_H__

#include <GLES3/gl3.h>
#include <stddef.h>
#include <stdint.h>

// Texture pixel format
enum PIXELSIZE {
	PIXELSIZE_BYTE = 1,
	PIXELSIZE_2BYTES = 2,
	PIXELSIZE_3BYTES = 3,
	PIXELSIZE_4BYTES = 4,

	PIXELSIZE_MAX
};

typedef struct Texture {
	GLuint id;
	uint32_t width;
	uint32_t height;
} Texture, *Texture_t;

Texture textureCreateFromMemoryRaw(const void *data, uint32_t width, uint32_t height, enum PIXELSIZE pixelSize);
void textureCreateFromMemoryRawInplace(Texture_t texture, const void *data, uint32_t width, uint32_t height, enum PIXELSIZE pixelSize);
Texture textureCreateFromMemory(const uint8_t *data, size_t size);
void textureCreateFromMemoryInplace(Texture_t texture, const uint8_t *data, size_t size);
Texture textureLoadFromFile(const char *filename);
void textureLoadFromFileInplace(Texture_t texture, const char *filename);
void textureDestroy(Texture_t texture);
void textureDelete(Texture_t *texture);

#endif
