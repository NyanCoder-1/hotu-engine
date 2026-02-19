#ifndef __SCENE_SCENE_H__
#define __SCENE_SCENE_H__

#include "core.type.h"
#include "render/camera.h"
#include "render/material.h"
#include "render/mesh.h"
#include "render/shader.h"
#include "render/texture.h"
#include "scene/scene-object.h"
#include <stddef.h>
#include <stdint.h>

typedef struct Scene *Scene_t;
typedef const struct Scene *SceneConst_t;
typedef void (*SceneOnDelete_t)(void *userData);
typedef void (*SceneOnUpdate_t)(void *userData, double deltaTime);

Scene_t sceneNew(Core_t core);
void sceneDelete(Scene_t *scene);
void sceneUpdate(Scene_t self, double deltaTime);

void sceneSetUserData(Scene_t self, void *userData, SceneOnDelete_t onDelete);
void sceneSetOnUpdate(Scene_t self, SceneOnUpdate_t onUpdate);
void *sceneGetUserData(SceneConst_t self);

Core_t sceneGetCore(SceneConst_t self);

void sceneSetCamera(Scene_t self, Camera_t camera);
Camera sceneGetCamera(SceneConst_t self);

size_t sceneGetObjectsCount(SceneConst_t self);
SceneObject_t sceneGetObjectArray(Scene_t self);
size_t sceneGetUIElementsCount(SceneConst_t self);
SceneUIElement_t sceneGetUIElementArray(Scene_t self);

uint32_t sceneCreateObject(Scene_t self, const char *name);
uint32_t sceneCreateObjectInitialize(Scene_t self, const char *name, const SceneObject objectInitData);

uint32_t sceneCreateMaterial(Scene_t self, const char *name);
uint32_t sceneCreateMesh(Scene_t self, const char *name);
uint32_t sceneCreateShader(Scene_t self, const char *name);
uint32_t sceneCreateTexture(Scene_t self, const char *name);
Material_t sceneGetMaterial(SceneConst_t self, uint32_t id);
Material_t sceneGetMaterialByName(SceneConst_t self, const char *name);
Mesh_t sceneGetMesh(SceneConst_t self, uint32_t id);
Mesh_t sceneGetMeshByName(SceneConst_t self, const char *name);
Shader_t sceneGetShader(SceneConst_t self, uint32_t id);
Shader_t sceneGetShaderByName(SceneConst_t self, const char *name);
Texture_t sceneGetTexture(SceneConst_t self, uint32_t id);
Texture_t sceneGetTextureByName(SceneConst_t self, const char *name);

#endif
