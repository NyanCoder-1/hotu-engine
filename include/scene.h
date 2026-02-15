#ifndef __SCENE_H__
#define __SCENE_H__

#include "scene-object.h"
#include <stddef.h>
#include <stdint.h>

typedef struct Scene *Scene_t;

Scene_t sceneNew();
void sceneDelete(Scene_t *scene);
void sceneUpdate(Scene_t self, double deltaTime);
size_t sceneGetObjectsCount(Scene_t self);
SceneObject_t sceneGetObjectArray(Scene_t self);
size_t sceneGetUIElementsCount(Scene_t self);
SceneUIElement_t sceneGetUIElementArray(Scene_t self);

#endif
