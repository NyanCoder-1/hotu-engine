#include "scene/scene.h"
#include "render/camera.h"
#include "scene/scene-object.h"
#include "scene/scene-renderer.h"
#include "utility/dynamic-array.h"
#include <stdlib.h>
#include <string.h>

typedef struct Scene {
	Core_t core;
	SceneRenderer_t renderer;
	SceneObject_t objects;
	SceneUIElement_t uiElements;
	Camera_t camera;

	void *userData;
	SceneOnDelete_t onDelete;
	SceneOnUpdate_t onUpdate;
} Scene;

enum {
	SCENE_OBJECTS_INITIAL_CAPACITY = 16,
	SCENE_UI_ELEMENTS_INITIAL_CAPACITY = 8,
};

Scene_t sceneNew(Core_t core) {
	Scene_t scene = (Scene_t)malloc(sizeof(Scene));
	if (!scene) return 0;

	memset(scene, 0, sizeof(Scene));
	scene->core = core;
	scene->renderer = sceneRendererNew(scene);
	scene->objects = dynamicArrayCreate(sizeof(SceneObject), SCENE_OBJECTS_INITIAL_CAPACITY);
	scene->uiElements = dynamicArrayCreate(sizeof(SceneUIElement), SCENE_UI_ELEMENTS_INITIAL_CAPACITY);

	return scene;
}
void sceneDelete(Scene_t *scene) {
	if (!scene || !*scene) return;

	if ((*scene)->onDelete) (*scene)->onDelete((*scene)->userData);
	memset(*scene, 0, sizeof(Scene));
	free(*scene);
	*scene = 0;
}
void sceneUpdate(Scene_t self, double deltaTime) {
	if (self->onUpdate)
		self->onUpdate(self->userData, deltaTime);
	sceneRendererFrame(self->renderer);
}
void sceneSetUserData(Scene_t self, void *userData, SceneOnDelete_t onDelete) {
	self->userData = userData;
	self->onDelete = onDelete;
}
void sceneSetOnUpdate(Scene_t self, SceneOnUpdate_t onUpdate) {
	self->onUpdate = onUpdate;
}
void *sceneGetUserData(SceneConst_t self) {
	return self->userData;
}

Core_t sceneGetCore(SceneConst_t self) {
	return self->core;
}

void sceneSetCamera(Scene_t self, Camera_t camera) {
	self->camera = camera;
}
Camera sceneGetCamera(SceneConst_t self) {
	return self->camera ? *self->camera : cameraDefault(self->core);
}

size_t sceneGetObjectsCount(SceneConst_t self) {
	return dynamicArrayGetSize(self->objects);
}
SceneObject_t sceneGetObjectArray(Scene_t self) {
	return self->objects;
}
size_t sceneGetUIElementsCount(SceneConst_t self) {
	return dynamicArrayGetSize(self->uiElements);
}
SceneUIElement_t sceneGetUIElementArray(Scene_t self) {
	return self->uiElements;
}

uint32_t sceneCreateObject(Scene_t self, const char *name) {
	return sceneCreateObjectInitialize(self, name, (SceneObject){0});
}
uint32_t sceneCreateObjectInitialize(Scene_t self, const char *name, const SceneObject objectInitData) {
	uint32_t result = dynamicArrayGetSize(self->objects);
	DYNAMIC_ARRAY_WRAP_REALLOC(self->objects, dynamicArrayPush, { return ~0; }, &objectInitData);
	return result;
}

uint32_t sceneCreateMaterial(Scene_t self, const char *name) {
	return sceneRendererCreateMaterial(self->renderer, name);
}
uint32_t sceneCreateMesh(Scene_t self, const char *name) {
	return sceneRendererCreateMesh(self->renderer, name);
}
uint32_t sceneCreateShader(Scene_t self, const char *name) {
	return sceneRendererCreateShader(self->renderer, name);
}
uint32_t sceneCreateTexture(Scene_t self, const char *name) {
	return sceneRendererCreateTexture(self->renderer, name);
}
Material_t sceneGetMaterial(SceneConst_t self, uint32_t id) {
	return sceneRendererGetMaterial(self->renderer, id);
}
Material_t sceneGetMaterialByName(SceneConst_t self, const char *name) {
	return sceneRendererGetMaterialByName(self->renderer, name);
}
Mesh_t sceneGetMesh(SceneConst_t self, uint32_t id) {
	return sceneRendererGetMesh(self->renderer, id);
}
Mesh_t sceneGetMeshByName(SceneConst_t self, const char *name) {
	return sceneRendererGetMeshByName(self->renderer, name);
}
Shader_t sceneGetShader(SceneConst_t self, uint32_t id) {
	return sceneRendererGetShader(self->renderer, id);
}
Shader_t sceneGetShaderByName(SceneConst_t self, const char *name) {
	return sceneRendererGetShaderByName(self->renderer, name);
}
Texture_t sceneGetTexture(SceneConst_t self, uint32_t id) {
	return sceneRendererGetTexture(self->renderer, id);
}
Texture_t sceneGetTextureByName(SceneConst_t self, const char *name) {
	return sceneRendererGetTextureByName(self->renderer, name);
}
