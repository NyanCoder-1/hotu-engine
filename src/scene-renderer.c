#include "scene-renderer.h"
#include "render/material.h"
#include "render/mesh.h"
#include "render/shader.h"
#include "render/texture.h"
#include "scene.h"
#include "utility/dynamic-array.h"
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

enum {
	SCENE_MESHES_INITIAL_CAPACITY = 16,
	SCENE_TEXTURES_INITIAL_CAPACITY = 16,
	SCENE_SHADERS_INITIAL_CAPACITY = 16,
	SCENE_MATERIALS_INITIAL_CAPACITY = 16,
};

static void initDefaults(SceneRenderer_t self);

SceneRenderer_t sceneRendererNew(Scene_t scene) {
	SceneRenderer_t renderer = (SceneRenderer_t)malloc(sizeof(Renderer));
	if (!renderer) return 0;

	memset(renderer, 0, sizeof(Renderer));
	renderer->scene = scene;
	renderer->bgColor = (struct Color){ .r = 0.0f, .g = 0.5f, .b = 0.5f, .a = 1.0f}; // Default background color

	renderer->meshes = dynamicArrayCreate(sizeof(Mesh), SCENE_MESHES_INITIAL_CAPACITY);
	renderer->textures = dynamicArrayCreate(sizeof(Texture), SCENE_TEXTURES_INITIAL_CAPACITY);
	renderer->shaders = dynamicArrayCreate(sizeof(Shader), SCENE_SHADERS_INITIAL_CAPACITY);
	renderer->materials = dynamicArrayCreate(sizeof(Material), SCENE_MATERIALS_INITIAL_CAPACITY);

	// Defaults
	DYNAMIC_ARRAY_WRAP_REALLOC(renderer->meshes, dynamicArrayResize, { sceneRendererDelete(&renderer); return 0; }, 1); // Default mesh
	DYNAMIC_ARRAY_WRAP_REALLOC(renderer->textures, dynamicArrayResize, { sceneRendererDelete(&renderer); return 0; }, 1); // Default texture
	DYNAMIC_ARRAY_WRAP_REALLOC(renderer->shaders, dynamicArrayResize, { sceneRendererDelete(&renderer); return 0; }, 1); // Default shader
	DYNAMIC_ARRAY_WRAP_REALLOC(renderer->materials, dynamicArrayResize, { sceneRendererDelete(&renderer); return 0; }, 1); // Default material
	initDefaults(renderer);

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

	size_t objectsCount = sceneGetObjectsCount(self->scene);
	SceneObject_t objects = sceneGetObjectArray(self->scene);
	// opaque objects pass
	for (size_t i = 0; i < objectsCount; i++) {
		SceneObject_t currentObject = &objects[i];
		if (!currentObject->visible || currentObject->transclucent) continue;

		materialBind(&self->materials[currentObject->materialId], self->shaders, self->textures);
		meshRender(&self->meshes[currentObject->meshId]);
	}
	// transparent objects pass
	for (size_t i = 0; i < objectsCount; i++) {
		SceneObject_t currentObject = &objects[i];
		if (!currentObject->visible || !currentObject->transclucent) continue;

		materialBind(&self->materials[currentObject->materialId], self->shaders, self->textures);
		meshRender(&self->meshes[currentObject->meshId]);
	}
}


typedef struct Vertex {
	Vector2f position;
	ColorRGBAf color;
} Vertex;
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

static void initDefaults(SceneRenderer_t self) {
	meshCreateInplace(&self->meshes[0], &vertexDescription, triangleVertices, sizeof(triangleVertices), triangleIndices, sizeof(triangleIndices), MESH_INDEX_TYPE_UINT8);
	textureLoadFromFileInplace(&self->textures[0], "assets/textures/texture.png");
	shaderCreateFromFileInplace(&self->shaders[0], "assets/shaders/vs.glsl", "assets/shaders/fs.glsl");
	materialCreateInplace(&self->materials[0], 0, 0);
}
