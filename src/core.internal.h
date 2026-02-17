#ifndef __CORE_COMMON_H__
#define __CORE_COMMON_H__

#include "app.h"
#include "scene/scene.h"
#include "scene/scene-manager.h"
#include <stdint.h>
#if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif
#include <time.h>

typedef struct CoreInternal {
	App_t app;
	SceneManager_t sceneManager;
	Scene_t currentScene;
	int width;
	int height;
	struct timespec timePrev;
} CoreInternal, *CoreInternal_t;

// Internal functions
void coreInternalCtor(CoreInternal_t self);
void coreInternalDtor(CoreInternal_t self);
void coreInternalFrame(CoreInternal_t self);
void coreInternalOnResize(CoreInternal_t self, int width, int height);
void coreInternalOnKeyDown(CoreInternal_t self, uint32_t key);
void coreInternalOnKeyUp(CoreInternal_t self, uint32_t key);
void coreInternalOnMouseDown(CoreInternal_t self, uint32_t button, double x, double y);
void coreInternalOnMouseUp(CoreInternal_t self, uint32_t button, double x, double y);
void coreInternalOnMouseMove(CoreInternal_t self, double x, double y);

CoreInternal_t coreGetInternal(Core_t self);

#endif

