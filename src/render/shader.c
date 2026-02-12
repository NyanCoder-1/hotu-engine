#include "render/shader.h"
#include "utility/asset-loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *const uniformNames[SHADER_UNIFORM_LOCATION_MAX] = {
	/* SHADER_UNIFORM_LOCATION_MATRIX_MODELVIEW           */ "matModelView",
	/* SHADER_UNIFORM_LOCATION_MATRIX_MODELVIEWPROJECTION */ "matModelViewProjection",
};
static const char *const attributeNames[SHADER_ATTRIBUTE_LOCATION_MAX] = {
	/* SHADER_ATTRIBUTE_LOCATION_POSITION           */ "inPos",
	/* SHADER_ATTRIBUTE_LOCATION_NORMAL             */ "inNorm",
	/* SHADER_ATTRIBUTE_LOCATION_TANGENT            */ "inTangent",
	/* SHADER_ATTRIBUTE_LOCATION_BITANGENT          */ "inBitangent",
	/* SHADER_ATTRIBUTE_LOCATION_TEXTURECOORDINATES */ "inTexCoord",
	/* SHADER_ATTRIBUTE_LOCATION_COLOR              */ "inColor",
};

static GLuint createShaderObject(const char *shaderSource, GLenum type);

Shader shaderCreateFromSource(const char *vertexShaderSource, const char *fragmentShaderSource) {
	Shader shader;
	shaderCreateFromSourceInplace(&shader, vertexShaderSource, fragmentShaderSource);
	return shader;
}
Shader shaderCreateFromFile(const char *vertexFilename, const char *fragmentFilename) {
	Shader shader;
	shaderCreateFromFileInplace(&shader, vertexFilename, fragmentFilename);
	return shader;
}
void shaderCreateFromSourceInplace(Shader_t shader, const char *vertexShaderSource, const char *fragmentShaderSource) {
	if (!shader || !vertexShaderSource || !fragmentShaderSource) {
		fprintf(stdout, "Error in %s: Invalid argument\n", __func__);
		return;
	}

	GLuint program = glCreateProgram();

	GLuint vShader = createShaderObject(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fShader = createShaderObject(fragmentShaderSource, GL_FRAGMENT_SHADER);

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	// Attribute locations (API)
	for (int i = 0; i < SHADER_ATTRIBUTE_LOCATION_MAX; i++) {
		glBindAttribLocation(program, i, attributeNames[i]);
	}

	glLinkProgram(program);

	// Predelete shader objects, so they automatically delete when the program is deleted
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	shader->program = program;
	for (int i = 0; i < SHADER_UNIFORM_LOCATION_MAX; i++) {
		shader->uniformLocations[i] = glGetUniformLocation(program, uniformNames[i]);
	}
}
void shaderCreateFromFileInplace(Shader_t shader, const char *vertexFilename, const char *fragmentFilename) {
	if (!shader || !vertexFilename || !fragmentFilename) {
		fprintf(stdout, "Error in %s: Invalid argument\n", __func__);
		return;
	}
	uint8_t *assetVertexShaderSource = loadAssetBinary(vertexFilename);
	uint8_t *assetFragmentShaderSource = loadAssetBinary(fragmentFilename);
	if (!assetVertexShaderSource || !assetFragmentShaderSource) {
		fprintf(stdout, "Error in %s: Failed to load asset binary\n", __func__);
		return;
	}
	size_t sizeVertex  = assetGetSize(assetVertexShaderSource);
	size_t sizeFragment = assetGetSize(assetFragmentShaderSource);
	if (sizeVertex == 0 || sizeFragment == 0) {
		fprintf(stdout, "Error in %s: Empty file\n", __func__);
		freeAssetBinary(assetVertexShaderSource);
		freeAssetBinary(assetFragmentShaderSource);
		return;
	}

	shaderCreateFromSourceInplace(shader, (const char*)assetVertexShaderSource, (const char *)assetFragmentShaderSource);
	freeAssetBinary(assetVertexShaderSource);
	freeAssetBinary(assetFragmentShaderSource);
}
void shaderDestroy(Shader_t shader) {
	if (!shader) return;
	if (shader->program)
		glDeleteShader(shader->program);
	memset(shader, 0, sizeof(Shader));
}
void shaderDelete(Shader_t *shader) {
	if (!shader) return;
	shaderDestroy(*shader);
	free(*shader);
	*shader = NULL;
}

static GLuint createShaderObject(const char *shaderSource, GLenum shaderType) {
	// Create shader
	GLuint shader = glCreateShader(shaderType);
	// Attach shader source
	glShaderSource(shader, 1, &shaderSource, NULL);
	// Compile shader
	glCompileShader(shader);
	// Check for errors
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		// Print error
		GLint maxLength	= 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar errorLog[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(shader);
		fprintf(stderr, "%s\n", &errorLog[0]);
	}
	return shader;
}