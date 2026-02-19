#ifndef __UTILITY_MATH_TYPES_H__
#define __UTILITY_MATH_TYPES_H__

#include "utility/align.h"
#include <stdint.h>

typedef struct Vector2f {
	float x;
	float y;
} Vector2f;

typedef struct Vector2i {
	int x;
	int y;
} Vector2i;
typedef struct Vector2ui32 {
	uint32_t x;
	uint32_t y;
} Vector2ui32;
typedef struct Vector2i32 {
	int32_t x;
	int32_t y;
} Vector2i32;

typedef struct Vector3f {
	float x;
	float y;
	float z;
} Vector3f;

typedef struct Quaternion {
	float w;
	float x;
	float y;
	float z;
} Vector4f;
typedef ALIGN(16) Vector4f Quaternion;

typedef ALIGN_MAT struct Matrix4x4 {
	union {
		float m[16];
		float mAlt[4][4];
		Vector4f vectors[4];
	};
} Matrix4x4;

typedef struct ColorRGBA {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} ColorRGBA;

typedef struct ColorRGBAf {
	float r;
	float g;
	float b;
	float a;
} ColorRGBAf;

#endif
