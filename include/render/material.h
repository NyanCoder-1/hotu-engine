#ifndef __RENDER_MATERIAL_H__
#define __RENDER_MATERIAL_H__

#include "render/shader.h"
#include "render/texture.h"
#include <stdint.h>

typedef struct Material {
	uint32_t shaderId;
	uint32_t textureId;
} Material, *Material_t;

Material materialCreate(uint32_t shaderId, uint32_t textureId);
void materialCreateInplace(Material_t material, uint32_t shaderId, uint32_t textureId);
void materialDestroy(Material_t material);
void materialDelete(Material_t *material);
void materialBind(Material_t self, Shader_t shaders, Texture_t textures);

#endif
