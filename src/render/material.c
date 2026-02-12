#include "render/material.h"
#include <stdlib.h>
#include <string.h>

Material materialCreate(uint32_t shaderId, uint32_t textureId) {
	Material material;
	materialCreateInplace(&material, shaderId, textureId);
	return material;
}
void materialCreateInplace(Material_t material, uint32_t shaderId, uint32_t textureId) {
	material->shaderId = shaderId;
	material->textureId = textureId;
}
void materialDestroy(Material_t material) {
	if (!material) return;

	memset(material, 0, sizeof(Material));
}
void materialDelete(Material_t *material) {
	if (!material) return;

	materialDestroy(*material);
	free(*material);
	*material = (Material_t)0;
}
void materialBind(Material_t self, Shader_t shaders, Texture_t textures) {
	glUseProgram(shaders[self->shaderId].program);
	glBindTexture(GL_TEXTURE_2D, textures[self->textureId].id);
}
