#ifndef __SCENE_OBJECTS_H__
#define __SCENE_OBJECTS_H__

typedef struct SceneObject *SceneObject_t;

SceneObject_t sceneObjectNew();
void sceneObjectDelete(SceneObject_t *model);

#endif
