#include "render/mesh.h"
#include <GLES3/gl3.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void applyAttributes(const MeshVertexDescription *vertexDescription);
static size_t getIndexSize(const uint8_t indexType);
static size_t convertIndexType(const uint8_t indexType);

Mesh meshCreate(const MeshVertexDescription *vertexDescription, const void *vertexData, const uint32_t vertexDataSize, const void *indicesData, const uint32_t indicesDataSize, const uint8_t indexType) {
	Mesh mesh;
	meshCreateInplace(&mesh, vertexDescription, vertexData, vertexDataSize, indicesData, indicesDataSize, indexType);
	return mesh;
}
void meshCreateInplace(Mesh_t mesh, const MeshVertexDescription *vertexDescription, const void *vertexData, const uint32_t vertexDataSize, const void *indicesData, const uint32_t indicesDataSize, const uint8_t indexType) {
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);
	GLuint indexBuffer = 0;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize, indicesData, GL_STATIC_DRAW);
	applyAttributes(vertexDescription);

	glBindVertexArray(0);

	mesh->vao = vertexArrayObject;
	mesh->vbo = vertexBuffer;
	mesh->ebo = indexBuffer;
	mesh->indicesCount = indicesDataSize / getIndexSize(indexType);
	mesh->indexType = indexType;
}
void meshDestroy(Mesh_t mesh) {
	if (!mesh) return;

	glBindVertexArray(0); // just in case if vao is bound
	if (mesh->vbo) glDeleteBuffers(1, &mesh->vbo);
	if (mesh->ebo) glDeleteBuffers(1, &mesh->ebo);
	if (mesh->vao) glDeleteVertexArrays(1, &mesh->vao);
	memset(mesh, 0, sizeof(Mesh));
}
void meshDelete(Mesh_t *mesh) {
	if (!mesh) return;

	meshDestroy(*mesh);
	free(*mesh);
	*mesh = (Mesh_t)0;
}
void meshUpdate(Mesh_t self, const void *vertexData, const uint32_t vertexDataSize, const void *indicesData, const uint32_t indicesDataSize, const uint8_t indexType) {
	if (!self || !self->vao) return;

	glBindVertexArray(self->vao);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize, indicesData, GL_STATIC_DRAW);
	glBindVertexArray(0);
	self->indicesCount = indicesDataSize / getIndexSize(indexType);
	self->indexType = indexType;
}
void meshUpdateVertices(Mesh_t self, const void *vertexData, const uint32_t vertexDataSize) {
	if (!self || !self->vao) return;

	glBindVertexArray(self->vao);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);
	glBindVertexArray(0);
}
void meshRender(Mesh_t self) {
	if (!self || !self->vao) return;

	glBindVertexArray(self->vao);
	glDrawElements(GL_TRIANGLES, self->indicesCount, convertIndexType(self->indexType), 0);
}

static void applyAttributes(const MeshVertexDescription *vertexDescription) {
	for (uint32_t i = 0; i < vertexDescription->attributesCount; i++) {
		glEnableVertexAttribArray(vertexDescription->attributes[i].location);
		glVertexAttribPointer(vertexDescription->attributes[i].location, vertexDescription->attributes[i].floatsCount, GL_FLOAT, GL_FALSE, vertexDescription->structureSize, vertexDescription->attributes[i].offset);
	}
}
static size_t getIndexSize(const uint8_t indexType) {
	size_t indexSize = sizeof(uint32_t);
	switch (indexType) {
		case MESH_INDEX_TYPE_UINT8: indexSize = sizeof(uint8_t); break;
		case MESH_INDEX_TYPE_UINT16: indexSize = sizeof(uint16_t); break;
	}
	return indexSize;
}
static size_t convertIndexType(const uint8_t indexType) {
	GLenum glType = GL_UNSIGNED_INT;
	switch (indexType) {
		case MESH_INDEX_TYPE_UINT8: glType = GL_UNSIGNED_BYTE; break;
		case MESH_INDEX_TYPE_UINT16: glType = GL_UNSIGNED_SHORT; break;
	}
	return glType;
}
