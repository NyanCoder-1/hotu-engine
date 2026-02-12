#ifndef __UTILITY_MATH_TYPES_H__
#define __UTILITY_MATH_TYPES_H__

#include <stdint.h>

typedef struct Vector2f {
	float x;
	float y;
} Vector2f;

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
} Quaternion;

typedef struct Matrix4x4 {
	float m[16];
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
