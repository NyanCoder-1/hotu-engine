#ifndef __SCENE_SCENE_OBJECTS_H__
#define __SCENE_SCENE_OBJECTS_H__

#include "utility/math.types.h"
#include <stdint.h>

typedef struct SceneObject {
	Vector3f position;
	Quaternion rotation;
	Vector3f velocity;
	Vector3f acceleration;

	uint32_t meshId;
	uint32_t materialId;
	uint8_t visible : 1;
	uint8_t transclucent : 1;
	uint8_t gravity : 1;
} SceneObject, *SceneObject_t;

typedef struct SceneUIElement {
	Vector2f position;
	Vector2f size;
	ColorRGBA color;
	ColorRGBA backgroundColor;
	uint32_t meshId;
	uint32_t materialId;
	char *text;
	uint8_t visible : 1;
} SceneUIElement, *SceneUIElement_t;

#endif
