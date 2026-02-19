#include "core.h"
#include "core.internal.h"
#include "core.type.h"
#include "scene/scene-manager.h"
#include <GLES3/gl3.h>
#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void coreInternalCtor(CoreInternal_t self) {
	clock_gettime(CLOCK_MONOTONIC, &self->timePrev);
	self->sceneManager = sceneManagerNew();
}
void coreInternalDtor(CoreInternal_t self) {
	sceneManagerDelete(&self->sceneManager);
}
void coreInternalFrame(CoreInternal_t self) {
	struct timespec timeCurrent;
	clock_gettime(CLOCK_MONOTONIC, &timeCurrent); // Never use `gettimeofday()` for dTime, you could use that for current time, like for clock, but not for dTime
	double deltaTime = (timeCurrent.tv_sec - self->timePrev.tv_sec) + (timeCurrent.tv_nsec - self->timePrev.tv_nsec) / 1000000000.0;
	self->timePrev = timeCurrent;
	glViewport(0, 0, self->width, self->height);
	if (self->app)
		appUpdate(self->app, deltaTime);
	if (self->currentScene)
		sceneUpdate(self->currentScene, deltaTime);
}

void coreInternalOnResize(CoreInternal_t self, int width, int height) {
	self->width = width;
	self->height = height;
}
void coreInternalOnKeyDown(CoreInternal_t self, uint32_t key) {
}
void coreInternalOnKeyUp(CoreInternal_t self, uint32_t key) {
}
void coreInternalOnMouseDown(CoreInternal_t self, uint32_t button, double x, double y) {
}
void coreInternalOnMouseUp(CoreInternal_t self, uint32_t button, double x, double y) {
}
void coreInternalOnMouseMove(CoreInternal_t self, double x, double y) {
}

Vector2i coreGetScreenSize(const Core_t self) {
	return (Vector2i){ coreGetInternal(self)->width, coreGetInternal(self)->height };
}
Scene_t coreGetCurrentScene(Core_t self) {
	return coreGetInternal(self)->currentScene;
}
void coreAddScene(Core_t self, Scene_t scene, const char *name) {
	sceneManagerAddScene(coreGetInternal(self)->sceneManager, scene, name);
}
void coreRemoveScene(Core_t self, const char *name) {
	sceneManagerRemoveScene(coreGetInternal(self)->sceneManager, name);
}
void coreSetCurrentScene(Core_t self, const char *name) {
	CoreInternal_t internal = coreGetInternal(self);
	Scene_t scene = sceneManagerGetSceneByName(internal->sceneManager, name);
	if (scene) {
		internal->currentScene = scene;
	} else {
		fprintf(stderr, "Error [Core]: Scene not found\n");
	}
}
