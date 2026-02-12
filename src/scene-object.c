#include "scene-object.h"
#include <stdlib.h>
#include <string.h>

typedef struct SceneObject {
	int dummy;
} SceneObject;

SceneObject_t sceneObjectNew() {
	SceneObject_t model = (SceneObject_t)malloc(sizeof(SceneObject));
	if (!model) return 0;

	memset(model, 0, sizeof(SceneObject));

	return model;
}
void sceneObjectDelete(SceneObject_t *model) {
	if (!model || !*model) return;

	memset(*model, 0, sizeof(SceneObject));
	free(*model);
	*model = 0;
}

