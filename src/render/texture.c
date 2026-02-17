#include "render/texture.h"
#include "stb_image.h"
#include "utility/asset-loader.h"
#include <stdio.h>
#include <string.h>

Texture textureCreateFromMemoryRaw(const void *data, uint32_t width, uint32_t height, enum PIXELSIZE pixelSize) {
	Texture result;
	textureCreateFromMemoryRawInplace(&result, data, width, height, pixelSize);
	return result;
}
Texture textureCreateFromMemory(const uint8_t *data, size_t size) {
	Texture result;
	textureCreateFromMemoryInplace(&result, data, size);
	return result;
}
Texture textureLoadFromFile(const char *filename) {
	Texture result;
	textureLoadFromFileInplace(&result, filename);
	return result;
}
void textureCreateFromMemoryRawInplace(Texture_t texture, const void *data, uint32_t width, uint32_t height, enum PIXELSIZE pixelSize) {
	// Skip if no data or empty data
	if (!texture || !data || width == 0 || height == 0) {
		fprintf(stderr, "Error in `%s`: Invalid argument\n", __func__);
		return;
	}

	GLint internalFormat = GL_RGBA8;
	GLenum format = GL_RGBA;
	switch (pixelSize) {
	case PIXELSIZE_BYTE:
		internalFormat = GL_R8;
		format = GL_RED;
		break;
	case PIXELSIZE_2BYTES:
		internalFormat = GL_RG8;
		format = GL_RG;
		break;
	case PIXELSIZE_3BYTES:
		internalFormat = GL_RGB8;
		format = GL_RGB;
		break;
	case PIXELSIZE_4BYTES:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		break;
	default:
		fprintf(stderr, "Error in `%s`: Unsupported pixel size\n", __func__);
		return;
	}

	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
}
void textureCreateFromMemoryInplace(Texture_t texture, const uint8_t* data, size_t size) {
	// Skip if no data or empty data
	if (!texture || !data || size == 0) {
		fprintf(stderr, "Error in `%s`: Invalid argument\n", __func__);
		return;
	}

	int width, height, channels;
	uint8_t *img = stbi_load_from_memory(data, size, &width, &height, &channels, 0);
	enum PIXELSIZE pixelSize = 0;
	switch (channels) {
	case 1:
		pixelSize = PIXELSIZE_BYTE;
		break;
	case 2:
		pixelSize = PIXELSIZE_2BYTES;
		break;
	case 3:
		pixelSize = PIXELSIZE_3BYTES;
		break;
	case 4:
		pixelSize = PIXELSIZE_4BYTES;
		break;
	}
	if (width <= 0 || height <= 0 || channels <= 0 || channels >= PIXELSIZE_MAX) {
		fprintf(stderr, "Error in `%s`: Invalid image data or unsupported pixel format\n", __func__);
	}
	else {
		textureCreateFromMemoryRawInplace(texture, img, width, height, pixelSize);
	}
	stbi_image_free(img);
}
void textureLoadFromFileInplace(Texture_t texture, const char* filename) {
	// Skip if no data or empty filename
	if (!texture || !filename || !*filename) {
		fprintf(stderr, "Error in `%s`: Invalid argument\n", __func__);
		return;
	}

	uint8_t *fileData = loadAssetBinary(filename);
	if (!fileData) {
		fprintf(stderr, "Error in `%s`: Failed to load asset binary\n", __func__);
		return;
	}
	textureCreateFromMemoryInplace(texture, fileData, assetGetSize(fileData));
	freeAssetBinary(fileData);
}
void textureDestroy(Texture_t texture) {
	if (!texture) return;
	if (texture->id)
		glDeleteTextures(1, &texture->id);
	memset(texture, 0, sizeof(Texture));
}
void textureDelete(Texture_t *texture) {
	if (!texture) return;
	textureDestroy(*texture);
	free(*texture);
	*texture = 0;
}
