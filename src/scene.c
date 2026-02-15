#include "scene.h"
#include "scene-object.h"
#include "scene-renderer.h"
#include "utility/dynamic-array.h"
#include <stdlib.h>
#include <string.h>

typedef struct Scene {
	SceneRenderer_t renderer;
	SceneObject_t objects;
	SceneUIElement_t uiElements;
} Scene;

enum {
	SCENE_OBJECTS_INITIAL_CAPACITY = 16,
	SCENE_UI_ELEMENTS_INITIAL_CAPACITY = 8,
};

Scene_t sceneNew() {
	Scene_t scene = (Scene_t)malloc(sizeof(Scene));
	if (!scene) return 0;

	memset(scene, 0, sizeof(Scene));
	scene->renderer = sceneRendererNew(scene);
	scene->objects = dynamicArrayCreate(sizeof(SceneObject), SCENE_OBJECTS_INITIAL_CAPACITY);
	scene->uiElements = dynamicArrayCreate(sizeof(SceneUIElement), SCENE_UI_ELEMENTS_INITIAL_CAPACITY);

	dynamicArrayPush(scene->objects, (SceneObject[]){{ .position = {}, .rotation = {}, .meshId = 0, .materialId = 0, .visible = 1, .transclucent = 0 }});

	return scene;
}
void sceneDelete(Scene_t *scene) {
	if (!scene || !*scene) return;

	memset(*scene, 0, sizeof(Scene));
	free(*scene);
	*scene = 0;
}
void sceneUpdate(Scene_t self, double deltaTime) {
	(void)deltaTime;
	sceneRendererFrame(self->renderer);
}
size_t sceneGetObjectsCount(Scene_t self) {
	return dynamicArrayGetSize(self->objects);
}
SceneObject_t sceneGetObjectArray(Scene_t self) {
	return self->objects;
}
size_t sceneGetUIElementsCount(Scene_t self) {
	return dynamicArrayGetSize(self->uiElements);
}
SceneUIElement_t sceneGetUIElementArray(Scene_t self) {
	return self->uiElements;
}

