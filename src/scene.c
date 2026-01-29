#include "scene.h"
#include "model.h"
#include "view.h"
#include <stdlib.h>
#include <string.h>

typedef struct Scene {
	Model_t model;
	View_t view;
} Scene;

Scene_t sceneNew() {
	Scene_t scene = (Scene_t)malloc(sizeof(Scene));
	if (!scene) return 0;

	memset(scene, 0, sizeof(Scene));
	scene->model = modelNew();
	scene->view = viewNew(scene->model);

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
	viewRender(self->view);
}

