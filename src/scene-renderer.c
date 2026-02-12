#include "scene-renderer.h"
#include "render/material.h"
#include "render/mesh.h"
#include "render/shader.h"
#include "render/texture.h"
#include "scene.h"
#include "utility/math.types.h"
#include <GLES3/gl3.h>
#include <stdlib.h>
#include <string.h>

typedef struct SceneRenderer {
	struct Color {
		GLfloat r, g, b, a;
	} bgColor;
	Scene_t scene;

	Mesh_t meshes;
	Texture_t textures;
	Shader_t shaders;
	Material_t materials;
} Renderer;

typedef struct Vertex {
	Vector2f position;
	ColorRGBAf color;
} Vertex;
// typedef struct MeshVertexAttributeDescription {
// 	GLuint location;
// 	GLint floatsCount;
// 	const void *offset;
// } MeshVertexAttributeDescription, *MeshVertexAttributeDescription_t;
// typedef struct MeshVertexDescription {
// 	GLsizei structureSize;
// 	MeshVertexAttributeDescription_t attributes;
// 	uint32_t attributesCount;
// } MeshVertexDescription, *MeshVertexDescription_t;
static const MeshVertexAttributeDescription vertexAttributes[] = {
	(MeshVertexAttributeDescription){ .location = SHADER_ATTRIBUTE_LOCATION_POSITION, 2, (const void*)0 },
	(MeshVertexAttributeDescription){ .location = SHADER_ATTRIBUTE_LOCATION_COLOR, 4, (const void*)(sizeof(Vector2f)) },
};
static const MeshVertexDescription vertexDescription = {
	.structureSize = sizeof(Vertex),
	.attributes = vertexAttributes,
	.attributesCount = sizeof(vertexAttributes) / sizeof(MeshVertexAttributeDescription)
};
static const Vertex triangleVertices[] = {
	{ .position = { -1.0f, -1.0f }, .color = { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ .position = { 0.0f, 1.0f }, .color = { 0.0f, 1.0f, 0.0f, 1.0f } },
	{ .position = { 1.0f, -1.0f }, .color = { 0.0f, 0.0f, 1.0f, 1.0f } },
};
static const uint8_t triangleIndices[] = {
	0, 1, 2,
};

SceneRenderer_t sceneRendererNew(Scene_t scene) {
	SceneRenderer_t renderer = (SceneRenderer_t)malloc(sizeof(Renderer));
	if (!renderer) return 0;

	memset(renderer, 0, sizeof(Renderer));
	renderer->scene = scene;
	renderer->bgColor = (struct Color){ .r = 0.0f, .g = 0.5f, .b = 0.5f, .a = 1.0f}; // Default background color

	renderer->meshes = (Mesh_t)malloc(sizeof(Mesh) * 1);
	meshCreateInplace(&renderer->meshes[0], &vertexDescription, triangleVertices, sizeof(triangleVertices), triangleIndices, sizeof(triangleIndices), MESH_INDEX_TYPE_UINT8);
	renderer->textures = (Texture_t)malloc(sizeof(Texture) * 1);
	textureLoadFromFileInplace(&renderer->textures[0], "assets/textures/texture.png");
	renderer->shaders = (Shader_t)malloc(sizeof(Shader) * 1);
	shaderCreateFromFileInplace(&renderer->shaders[0], "assets/shaders/vs.glsl", "assets/shaders/fs.glsl");
	renderer->materials = (Material_t)malloc(sizeof(Material) * 1);
	materialCreateInplace(&renderer->materials[0], 0, 0);

	return renderer;
}
void sceneRendererDelete(SceneRenderer_t *renderer) {
	if (!renderer || !*renderer) return;

	memset(*renderer, 0, sizeof(Renderer));
	free(*renderer);
	*renderer = 0;
}
void sceneRendererFrame(SceneRenderer_t self) {
	glClearColor(self->bgColor.r, self->bgColor.g, self->bgColor.b, self->bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	materialBind(&self->materials[0], self->shaders, self->textures);
	meshRender(&self->meshes[0]);
}

