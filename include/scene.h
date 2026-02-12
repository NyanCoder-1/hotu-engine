#ifndef __SCENE_H__
#define __SCENE_H__

#include "scene-object.type.h"
#include <stdint.h>

typedef struct Scene *Scene_t;

Scene_t sceneNew();
void sceneDelete(Scene_t *scene);
void sceneUpdate(Scene_t self, double deltaTime);
SceneObject_t sceneGetObjectById(Scene_t self, uint32_t objectId);

#endif
