#include "scene-manager.h"
#include "stb_ds.h"

typedef struct {
	char *key;
	Scene_t value;
} ScenesPair;
typedef struct SceneManager {
	ScenesPair *scenes;
} SceneManager;

SceneManager_t sceneManagerNew() {
	SceneManager_t sceneManager = (SceneManager_t)malloc(sizeof(SceneManager));
	memset(sceneManager, 0, sizeof(SceneManager));
	return sceneManager;
}
void sceneManagerDelete(SceneManager_t *sceneManager) {
	if (!sceneManager || !*sceneManager) return;
	shfree((*sceneManager)->scenes);
	memset((*sceneManager), 0, sizeof(SceneManager));
	free(*sceneManager);
	*sceneManager = 0;
}
void sceneManagerAddScene(SceneManager_t self, Scene_t scene, const char *name) {
	shput(self->scenes, name, scene);
}
void sceneManagerRemoveScene(SceneManager_t self, const char *name) {
	shdel(self->scenes, name);
}
Scene_t sceneManagerGetSceneByName(SceneManager_t self, const char *name) {
	return shget(self->scenes, name);
}
