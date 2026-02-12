#ifndef __RENDER_MESH_H__
#define __RENDER_MESH_H__

#include <GLES3/gl3.h>
#include <stdint.h>

enum MESH_INDEX_TYPE {
	MESH_INDEX_TYPE_UINT8 = 0,
	MESH_INDEX_TYPE_UINT16 = 1,
	MESH_INDEX_TYPE_UINT32 = 2,
};

typedef struct Mesh {
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	uint32_t indicesCount;
	uint8_t indexType;
} Mesh, *Mesh_t;

// Assume all attributes are `float`
typedef struct MeshVertexAttributeDescription {
	GLuint location;
	GLint floatsCount;
	const void *offset;
} MeshVertexAttributeDescription, *MeshVertexAttributeDescription_t;
typedef struct MeshVertexDescription {
	GLsizei structureSize;
	const MeshVertexAttributeDescription *attributes;
	uint32_t attributesCount;
} MeshVertexDescription, *MeshVertexDescription_t;

Mesh meshCreate(const MeshVertexDescription *vertexDescription, const void *vertexData, const uint32_t vertexDataSize, const void *indicesData, const uint32_t indicesDataSize, const uint8_t indexType);
void meshCreateInplace(Mesh_t mesh, const MeshVertexDescription *vertexDescription, const void *vertexData, const uint32_t vertexDataSize, const void *indicesData, const uint32_t indicesDataSize, const uint8_t indexType);
void meshDestroy(Mesh_t mesh);
void meshDelete(Mesh_t *mesh);
void meshUpdate(Mesh_t self, const void *vertexData, const uint32_t vertexDataSize, const void *indicesData, const uint32_t indicesDataSize, const uint8_t indexType);
void meshUpdateVertices(Mesh_t self, const void *vertexData, const uint32_t vertexDataSize);
void meshRender(Mesh_t self);

#endif
