#ifndef __SCENE_SCENE_H__
#define __SCENE_SCENE_H__

#include "render/material.h"
#include "render/mesh.h"
#include "render/shader.h"
#include "render/texture.h"
#include "scene/scene-object.h"
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

uint32_t sceneCreateObject(Scene_t self, const char *name);
uint32_t sceneCreateObjectInitialize(Scene_t self, const char *name, const SceneObject objectInitData);

uint32_t sceneCreateMaterial(Scene_t self, const char *name);
uint32_t sceneCreateMesh(Scene_t self, const char *name);
uint32_t sceneCreateShader(Scene_t self, const char *name);
uint32_t sceneCreateTexture(Scene_t self, const char *name);
Material_t sceneGetMaterial(Scene_t self, uint32_t id);
Material_t sceneGetMaterialByName(Scene_t self, const char *name);
Mesh_t sceneGetMesh(Scene_t self, uint32_t id);
Mesh_t sceneGetMeshByName(Scene_t self, const char *name);
Shader_t sceneGetShader(Scene_t self, uint32_t id);
Shader_t sceneGetShaderByName(Scene_t self, const char *name);
Texture_t sceneGetTexture(Scene_t self, uint32_t id);
Texture_t sceneGetTextureByName(Scene_t self, const char *name);

#endif
