#include "render/camera.h"
#include "cglm/cam.h"
#include "cglm/euler.h"
#include "cglm/mat4.h"
#include "cglm/quat.h"
#include "cglm/util.h"
#include "core.h"
#include "utility/math.types.h"
#include <float.h>

Camera cameraDefault(Core_t core) {
	//Vector2i screenSize = coreGetScreenSize(core);
	return (Camera){
		.position = {0.0f, 0.0f, 0.0f},
		.rotation = GLM_QUAT_IDENTITY_INIT,
		.fov = 75.0f,
		//.aspectRatio = screenSize.y > FLT_EPSILON ? (float)screenSize.x / screenSize.y : FLT_MAX,
		.nearPlane = 0.1f,
		.farPlane = 100.0f
	};
}
void cameraSetPosition(Camera_t camera, const Vector3f position) {
	camera->position = position;
}
void cameraLookAt(Camera_t camera, const Vector3f target) {
	glm_quat_forp((float*)&camera->position, (float*)&target, (float[3]){0.0f, 0.0f, 1.0f}, (float*)&camera->rotation);
}
void cameraLookAtFrom(Camera_t camera, const Vector3f position, const Vector3f target) {
	camera->position = position;
	cameraLookAt(camera, target);
}
void cameraLookDirection(Camera_t camera, const Vector3f directionRowPitchYaw) {
	glm_euler_zyx_quat((float*)&directionRowPitchYaw, (float*)&camera->rotation);
}
void cameraLookDirectionFrom(Camera_t camera, const Vector3f position, const Vector3f directionRowPitchYaw) {
	camera->position = position;
	cameraLookDirection(camera, directionRowPitchYaw);
}
Matrix4x4 cameraGetViewMatrix(CameraConst_t camera) {
	Matrix4x4 mat = (Matrix4x4){ .mAlt = GLM_MAT4_IDENTITY_INIT };
	glm_quat_look(*(vec3*)&camera->position, *(vec4*)&camera->rotation, (vec4*)mat.mAlt);
	return mat;
}
Matrix4x4 cameraGetProjectionMatrix(CameraConst_t camera, Core_t core) {
	Matrix4x4 mat = (Matrix4x4){ .mAlt = GLM_MAT4_IDENTITY_INIT };
	Vector2i screenSize = coreGetScreenSize(core);
	double aspectRatio = screenSize.y > FLT_EPSILON ? (float)screenSize.x / screenSize.y : FLT_MAX;
	glm_perspective(glm_rad(camera->fov), aspectRatio, camera->nearPlane, camera->farPlane, (vec4*)mat.mAlt);
	return mat;
}
Matrix4x4 cameraGetViewProjectionMatrix(CameraConst_t camera, Core_t core) {
	Matrix4x4 viewMat = cameraGetViewMatrix(camera);
	Matrix4x4 projMat = cameraGetProjectionMatrix(camera, core);
	Matrix4x4 result = (Matrix4x4){ .mAlt = GLM_MAT4_IDENTITY_INIT };
	glm_mat4_mul((vec4*)projMat.mAlt, (vec4*)viewMat.mAlt, (vec4*)result.mAlt);
	return result;
}
