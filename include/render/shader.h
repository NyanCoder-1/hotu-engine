#ifndef __RENDER_SHADER_H__
#define __RENDER_SHADER_H__

#include <GLES3/gl3.h>

enum SHADER_UNIFORM_LOCATION {
	SHADER_UNIFORM_LOCATION_MATRIX_MODEL,
	SHADER_UNIFORM_LOCATION_MATRIX_MODELVIEWPROJECTION,

	SHADER_UNIFORM_LOCATION_MAX
};

enum SHADER_ATTRIBUTE_LOCATION {
	SHADER_ATTRIBUTE_LOCATION_POSITION,
	SHADER_ATTRIBUTE_LOCATION_NORMAL,
	SHADER_ATTRIBUTE_LOCATION_TANGENT,
	SHADER_ATTRIBUTE_LOCATION_BITANGENT,
	SHADER_ATTRIBUTE_LOCATION_TEXTURECOORDINATES,
	SHADER_ATTRIBUTE_LOCATION_COLOR,

	SHADER_ATTRIBUTE_LOCATION_MAX
};

typedef struct Shader {
	GLuint program;
	GLint uniformLocations[SHADER_UNIFORM_LOCATION_MAX];
} Shader, *Shader_t;

Shader shaderCreateFromSource(const char *vertexSource, const char *fragmentSource);
void shaderCreateFromSourceInplace(Shader_t shader, const char *vertexSource, const char *fragmentSource);
Shader shaderCreateFromFile(const char *vertexFilename, const char *fragmentFilename);
void shaderCreateFromFileInplace(Shader_t shader, const char *vertexFilename, const char *fragmentFilename);
void shaderDestroy(Shader_t shader);
void shaderDelete(Shader_t *shader);

#endif
