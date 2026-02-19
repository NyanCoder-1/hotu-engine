#ifndef __RENDER_CAMERA_H__
#define __RENDER_CAMERA_H__

#include "core.type.h"
#include "utility/math.types.h"

typedef struct Camera {
	Vector3f position;
	Quaternion rotation;
	float fov; // Field of view in degrees
	/* calculated in runtime */ //float aspectRatio; // Aspect ratio of the viewport
	float nearPlane; // Distance to the near clipping plane
	float farPlane; // Distance to the far clipping plane
} Camera, *Camera_t;
typedef const struct Camera *CameraConst_t;
#undef _ALIGN

Camera cameraDefault(Core_t core);
void cameraSetPosition(Camera_t camera, const Vector3f position);
void cameraLookAt(Camera_t camera, const Vector3f target);
void cameraLookAtFrom(Camera_t camera, const Vector3f position, const Vector3f target);
void cameraLookDirection(Camera_t camera, const Vector3f directionRowPitchYaw);
void cameraLookDirectionFrom(Camera_t camera, const Vector3f position, const Vector3f directionRowPitchYaw);
Matrix4x4 cameraGetViewMatrix(CameraConst_t camera);
Matrix4x4 cameraGetProjectionMatrix(CameraConst_t camera, Core_t core);
Matrix4x4 cameraGetViewProjectionMatrix(CameraConst_t camera, Core_t core);

#endif
