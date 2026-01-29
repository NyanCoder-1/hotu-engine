#ifndef __CORE_H__
#define __CORE_H__

#include "core.type.h"
#include "app.h"
#include "scene.h"

Core_t coreNew();
void coreDelete(Core_t *self);
void coreRun(Core_t self, App_t app);

// Scene management functions
Scene_t coreGetCurrentScene(Core_t self);
void coreAddScene(Core_t self, Scene_t scene, const char* name);
void coreRemoveScene(Core_t self, const char* name);
void coreSetCurrentScene(Core_t self, const char* name);

#endif

