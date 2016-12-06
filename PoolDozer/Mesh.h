#pragma once
#include "GL/glew.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <iostream>
#include <vector>

class CMesh
{
public:
	CMesh();

	~CMesh();

	bool LoadMesh(const std::string& Filename);

private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	//bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	//void Clear();

	struct Mesh {
		GLuint VBO;
		GLuint EBO;
		GLuint VAO;
		unsigned int numIndices;
		unsigned int numVertices;
		unsigned int materialIndex;
	};

	std::vector<Mesh> m_meshes;
	std::vector<GLuint> m_textures;
};