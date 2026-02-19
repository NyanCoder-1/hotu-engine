#include "scene/scene-renderer.h"
#include "cglm/mat4.h"
#include "cglm/quat.h"
#include "render/camera.h"
#include "render/material.h"
#include "render/mesh.h"
#include "render/shader.h"
#include "render/texture.h"
#include "scene/scene.h"
#include "stb_ds.h"
#include "utility/dynamic-array.h"
#include "utility/math.types.h"
#include <GLES3/gl3.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct PairNameId {
	const char *key;
	uint32_t value;
} PairNameId;

typedef struct SceneRenderer {
	struct Color {
		GLfloat r, g, b, a;
	} bgColor;
	Scene_t scene;

	Mesh_t meshes;
	Texture_t textures;
	Shader_t shaders;
	Material_t materials;

	PairNameId *meshesMap;
	PairNameId *texturesMap;
	PairNameId *shadersMap;
	PairNameId *materialsMap;
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
	if (!renderer) return NULL;

	memset(renderer, 0, sizeof(Renderer));
	renderer->scene = scene;
	renderer->bgColor = (struct Color){ .r = 0.0f, .g = 0.5f, .b = 0.5f, .a = 1.0f}; // Default background color

	renderer->meshes = dynamicArrayCreate(sizeof(Mesh), SCENE_MESHES_INITIAL_CAPACITY);
	renderer->textures = dynamicArrayCreate(sizeof(Texture), SCENE_TEXTURES_INITIAL_CAPACITY);
	renderer->shaders = dynamicArrayCreate(sizeof(Shader), SCENE_SHADERS_INITIAL_CAPACITY);
	renderer->materials = dynamicArrayCreate(sizeof(Material), SCENE_MATERIALS_INITIAL_CAPACITY);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera camera = sceneGetCamera(self->scene);
	Matrix4x4 matrixViewProjection = cameraGetViewProjectionMatrix(&camera, sceneGetCore(self->scene));

	size_t objectsCount = sceneGetObjectsCount(self->scene);
	SceneObject_t objects = sceneGetObjectArray(self->scene);
	// opaque objects pass
	for (size_t i = 0; i < objectsCount; i++) {
		SceneObject_t currentObject = &objects[i];
		if (!currentObject->visible || currentObject->transclucent) continue;

		mat4 rotation = GLM_MAT4_IDENTITY_INIT;
		glm_quat_mat4((float*)&currentObject->rotation, rotation);
		mat4 translation = GLM_MAT4_IDENTITY_INIT;
		glm_translate(translation, (float*)&currentObject->position);
		mat4 matrixModel = GLM_MAT4_IDENTITY_INIT;
		glm_mat4_mul(translation, rotation, matrixModel);
		mat4 matrixModelViewProjection = GLM_MAT4_IDENTITY_INIT;
		glm_mat4_mul((vec4*)matrixViewProjection.mAlt, matrixModel, matrixModelViewProjection);

		materialBind(&self->materials[currentObject->materialId], self->shaders, self->textures);
		Shader_t currentShader = &self->shaders[self->materials[currentObject->materialId].shaderId];
		glUniformMatrix4fv(currentShader->uniformLocations[SHADER_UNIFORM_LOCATION_MATRIX_MODEL], 1, GL_TRUE, (float*)matrixModel);
		glUniformMatrix4fv(currentShader->uniformLocations[SHADER_UNIFORM_LOCATION_MATRIX_MODELVIEWPROJECTION], 1, GL_TRUE, (float*)&matrixModelViewProjection);
		meshRender(&self->meshes[currentObject->meshId]);
	}
	// transparent objects pass
	for (size_t i = 0; i < objectsCount; i++) {
		SceneObject_t currentObject = &objects[i];
		if (!currentObject->visible || !currentObject->transclucent) continue;

		mat4 rotation = GLM_MAT4_IDENTITY_INIT;
		glm_quat_mat4t((float*)&currentObject->rotation, rotation);
		mat4 translation = GLM_MAT4_IDENTITY_INIT;
		glm_translate(translation, (float*)&currentObject->position);
		mat4 matrixModel = GLM_MAT4_IDENTITY_INIT;
		glm_mat4_mul(rotation, translation, matrixModel);
		mat4 matrixModelViewProjection = GLM_MAT4_IDENTITY_INIT;
		glm_mat4_mul((vec4*)matrixViewProjection.mAlt, matrixModel, matrixModelViewProjection);

		materialBind(&self->materials[currentObject->materialId], self->shaders, self->textures);
		Shader_t currentShader = &self->shaders[self->materials[currentObject->materialId].shaderId];
		glUniformMatrix4fv(currentShader->uniformLocations[SHADER_UNIFORM_LOCATION_MATRIX_MODEL], 1, GL_TRUE, (float*)matrixModel);
		glUniformMatrix4fv(currentShader->uniformLocations[SHADER_UNIFORM_LOCATION_MATRIX_MODELVIEWPROJECTION], 1, GL_TRUE, (float*)&matrixModelViewProjection);
		meshRender(&self->meshes[currentObject->meshId]);
	}
}

uint32_t sceneRendererCreateMaterial(SceneRenderer_t self, const char *name) {
	uint32_t result = dynamicArrayGetSize(self->materials);
	DYNAMIC_ARRAY_WRAP_REALLOC(self->materials, dynamicArrayPush, { return ~0; }, &(Material){});
	if (name) shput(self->materialsMap, name, result);
	return result;
}
uint32_t sceneRendererCreateMesh(SceneRenderer_t self, const char *name) {
	uint32_t result = dynamicArrayGetSize(self->materials);
	DYNAMIC_ARRAY_WRAP_REALLOC(self->meshes, dynamicArrayPush, { return ~0; }, &(Mesh){});
	if (name) shput(self->meshesMap, name, result);
	return result;
}
uint32_t sceneRendererCreateShader(SceneRenderer_t self, const char *name) {
	uint32_t result = dynamicArrayGetSize(self->materials);
	DYNAMIC_ARRAY_WRAP_REALLOC(self->shaders, dynamicArrayPush, { return ~0; }, &(Shader){});
	if (name) shput(self->shadersMap, name, result);
	return result;
}
uint32_t sceneRendererCreateTexture(SceneRenderer_t self, const char *name) {
	uint32_t result = dynamicArrayGetSize(self->materials);
	DYNAMIC_ARRAY_WRAP_REALLOC(self->textures, dynamicArrayPush, { return ~0; }, &(Texture){});
	if (name) shput(self->texturesMap, name, result);
	return result;
}
Material_t sceneRendererGetMaterial(SceneRenderer_t self, uint32_t id) {
	if (id >= dynamicArrayGetSize(self->materials)) return NULL;
	return &self->materials[id];
}
Mesh_t sceneRendererGetMesh(SceneRenderer_t self, uint32_t id) {
	if (id >= dynamicArrayGetSize(self->meshes)) return NULL;
	return &self->meshes[id];
}
Shader_t sceneRendererGetShader(SceneRenderer_t self, uint32_t id) {
	if (id >= dynamicArrayGetSize(self->shaders)) return NULL;
	return &self->shaders[id];
}
Texture_t sceneRendererGetTexture(SceneRenderer_t self, uint32_t id) {
	if (id >= dynamicArrayGetSize(self->textures)) return NULL;
	return &self->textures[id];
}
Material_t sceneRendererGetMaterialByName(SceneRenderer_t self, const char *name) {
	PairNameId *pair = shgetp(self->materialsMap, name);
	return pair ? sceneRendererGetMaterial(self, pair->value) : NULL;
}
Mesh_t sceneRendererGetMeshByName(SceneRenderer_t self, const char *name) {
	PairNameId *pair = shgetp(self->meshesMap, name);
	return pair ? sceneRendererGetMesh(self, pair->value) : NULL;
}
Shader_t sceneRendererGetShaderByName(SceneRenderer_t self, const char *name) {
	PairNameId *pair = shgetp(self->shadersMap, name);
	return pair ? sceneRendererGetShader(self, pair->value) : NULL;
}
Texture_t sceneRendererGetTextureByName(SceneRenderer_t self, const char *name) {
	PairNameId *pair = shgetp(self->texturesMap, name);
	return pair ? sceneRendererGetTexture(self, pair->value) : NULL;
}


typedef struct Vertex {
	Vector3f position;
	Vector2f uv;
	ColorRGBAf color;
} Vertex;
static const MeshVertexAttributeDescription vertexAttributes[] = {
	(MeshVertexAttributeDescription){ .location = SHADER_ATTRIBUTE_LOCATION_POSITION, 3, (const void*)offsetof(Vertex, position) },
	(MeshVertexAttributeDescription){ .location = SHADER_ATTRIBUTE_LOCATION_TEXTURECOORDINATES, 2, (const void*)offsetof(Vertex, uv) },
	(MeshVertexAttributeDescription){ .location = SHADER_ATTRIBUTE_LOCATION_COLOR, 4, (const void*)offsetof(Vertex, color) },
};
static const MeshVertexDescription vertexDescription = {
	.structureSize = sizeof(Vertex),
	.attributes = vertexAttributes,
	.attributesCount = sizeof(vertexAttributes) / sizeof(MeshVertexAttributeDescription)
};
static const Vertex triangleVertices[] = {
	{ .position = { -1.0f, 0.0f, 0.0f }, .uv = {0.0f, 0.5f}, .color = { 0.0f, 0.5f, 0.5f, 1.0f } },
	{ .position = { 0.0f, -1.0f, 0.0f }, .uv = {0.5f, 0.0f}, .color = { 0.5f, 0.0f, 0.5f, 1.0f } },
	{ .position = { 1.0f, 0.0f, 0.0f }, .uv = {1.0f, 0.5f}, .color = { 1.0f, 0.5f, 0.5f, 1.0f } },
	{ .position = { 0.0f, 1.0f, 0.0f }, .uv = {0.5f, 1.0f}, .color = { 0.5f, 1.0f, 0.5f, 1.0f } },
	{ .position = { 0.0f, 0.0f, -1.0f }, .uv = {0.5f, 0.5f}, .color = { 0.5f, 0.5f, 0.0f, 1.0f } },
	{ .position = { 0.0f, 0.0f, 1.0f }, .uv = {0.5f, 0.5f}, .color = { 0.05, 0.5f, 1.0f, 1.0f } },
};
static const uint8_t triangleIndices[] = {
	0, 1, 4,
	0, 5, 1,
	1, 2, 4,
	1, 5, 2,
	2, 3, 4,
	2, 5, 3,
	3, 0, 4,
	3, 5, 0,
};
static const char *const vertexShaderSource = "#version 300 es\n"
"uniform mat4 matModel;\n"
"uniform mat4 matModelViewProjection;\n"
"in vec3 inPos;\n"
"in vec2 inTexCoord;\n"
"in vec4 inColor;\n"
"out vec2 uv;\n"
"out vec4 color;\n"
"void main() {\n"
"	gl_Position = vec4(inPos, 1.0) * matModelViewProjection;\n"
"	uv = inTexCoord;\n"
"	color = inColor;\n"
"}";
static const char *const fragmentShaderSource = "#version 300 es\n"
"precision mediump float;\n"
"in vec2 uv;\n"
"in vec4 color;\n"
"out vec4 outColor;\n"
"uniform sampler2D texture0;\n"
"void main() {\n"
"	outColor = texture(texture0, uv) * color;\n"
"}";

static void initDefaults(SceneRenderer_t self) {
	uint32_t defaultMeshId = sceneRendererCreateMesh(self, "default");
	Mesh_t defaultMesh = sceneRendererGetMesh(self, defaultMeshId);
	meshCreateInplace(defaultMesh, &vertexDescription, triangleVertices, sizeof(triangleVertices), triangleIndices, sizeof(triangleIndices), MESH_INDEX_TYPE_UINT8);

	uint32_t defaultTextureId = sceneRendererCreateTexture(self, "default");
	Texture_t defaultTexture = sceneRendererGetTexture(self, defaultTextureId);
	// blackpink checkered pattern
	static const uint32_t textureWidth = 64, textureHeight = 64;
	ColorRGBA *textureData = (ColorRGBA *)malloc(textureWidth * textureHeight * sizeof(ColorRGBA));
	for (uint32_t i = 0; i < textureWidth * textureHeight; i++) {
		const uint32_t x = i % textureWidth;
		const uint32_t y = i / textureWidth;
		textureData[i] = (((x / 8) % 2) == ((y / 8) % 2)) ? (ColorRGBA){0xFF, 0xFF, 0xFF, 0xFF} : (ColorRGBA){0x00, 0x00, 0x00, 0xFF};
	}
	textureCreateFromMemoryRawInplace(defaultTexture, textureData, textureWidth, textureHeight, PIXELSIZE_4BYTES);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	free(textureData);

	uint32_t defaultShaderId = sceneRendererCreateShader(self, "default");
	Shader_t defaultShader = sceneRendererGetShader(self, defaultShaderId);
	shaderCreateFromSourceInplace(defaultShader, vertexShaderSource, fragmentShaderSource);

	uint32_t defaultMaterialId = sceneRendererCreateMaterial(self, "default");
	Material_t defaultMaterial = sceneRendererGetMaterial(self, defaultMaterialId);
	materialCreateInplace(defaultMaterial, defaultShaderId, defaultTextureId);
}
