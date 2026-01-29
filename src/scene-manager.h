#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "scene.h"

typedef struct SceneManager *SceneManager_t;

SceneManager_t sceneManagerNew();
void sceneManagerDelete(SceneManager_t *sceneManager);
void sceneManagerAddScene(SceneManager_t self, Scene_t scene, const char* name);
void sceneManagerRemoveScene(SceneManager_t self, const char* name);
Scene_t sceneManagerGetSceneByName(SceneManager_t self, const char* name);

#endif

