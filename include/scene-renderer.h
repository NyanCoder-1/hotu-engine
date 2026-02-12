#ifndef __SCENE_RENDERER_H__
#define __SCENE_RENDERER_H__

#include "scene.h"

typedef struct SceneRenderer *SceneRenderer_t;

SceneRenderer_t sceneRendererNew(Scene_t scene);
void sceneRendererDelete(SceneRenderer_t *view);
void sceneRendererFrame(SceneRenderer_t self);

#endif
