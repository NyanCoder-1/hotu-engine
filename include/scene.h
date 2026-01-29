#ifndef __SCENE_H__
#define __SCENE_H__

typedef struct Scene *Scene_t;

Scene_t sceneNew();
void sceneDelete(Scene_t *scene);
void sceneUpdate(Scene_t self, double deltaTime);

#endif
