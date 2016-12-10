#pragma once
#include "GL/glew.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "vertexData.h"

struct MeshStruct {
	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
	unsigned int numIndices;
	unsigned int numVertices;
	unsigned int materialIndex;
};
class CMesh
{
public:
	CMesh();

	~CMesh();

	bool LoadMesh(const std::string& Filename);
	void CleaBuffer();
	const std::vector<MeshStruct>& GetMeshData();
private:

	std::vector<MeshStruct> m_meshes;
	std::vector<GLuint> m_textures;
};