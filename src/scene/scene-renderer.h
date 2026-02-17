#ifndef __SCENE_SCENE_RENDERER_H__
#define __SCENE_SCENE_RENDERER_H__

#include "scene/scene.h"

typedef struct SceneRenderer *SceneRenderer_t;

SceneRenderer_t sceneRendererNew(Scene_t scene);
void sceneRendererDelete(SceneRenderer_t *view);
void sceneRendererFrame(SceneRenderer_t self);

uint32_t sceneRendererCreateMaterial(SceneRenderer_t self, const char *name);
uint32_t sceneRendererCreateMesh(SceneRenderer_t self, const char *name);
uint32_t sceneRendererCreateShader(SceneRenderer_t self, const char *name);
uint32_t sceneRendererCreateTexture(SceneRenderer_t self, const char *name);
Material_t sceneRendererGetMaterial(SceneRenderer_t self, uint32_t id);
Mesh_t sceneRendererGetMesh(SceneRenderer_t self, uint32_t id);
Shader_t sceneRendererGetShader(SceneRenderer_t self, uint32_t id);
Texture_t sceneRendererGetTexture(SceneRenderer_t self, uint32_t id);
Material_t sceneRendererGetMaterialByName(SceneRenderer_t self, const char *name);
Mesh_t sceneRendererGetMeshByName(SceneRenderer_t self, const char *name);
Shader_t sceneRendererGetShaderByName(SceneRenderer_t self, const char *name);
Texture_t sceneRendererGetTextureByName(SceneRenderer_t self, const char *name);

#endif
