#pragma once
#include "GL/glew.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <vector>
#include <iostream>
#include "FreeImage/FreeImage.h"
//#include "vertexData.h"

struct MeshStruct {
	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
	unsigned int numIndices;
	unsigned int numVertices;
	unsigned int materialIndex;
};
struct vertexData
{
	//vertex position
	float x;
	float y;
	float z;

	//texture mapping
	float tx;
	float ty;

	//Normals
	float nx;
	float ny;
	float nz;
	float nw;
};
//there is need for Mesh class because now Cmodel hold vector ofe textureStructs for all the Meshes

struct TextureStruct {
	GLuint id;
	std::string type;
	aiString path;
};

class CMesh
{
	friend class CModel;
public:
	CMesh() {}
	~CMesh() { UnloadOpenGLData(); }

	std::vector<vertexData> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<TextureStruct> m_textures;
	bool HasTextures() const { return !m_textures.empty(); }
	MeshStruct m_mesh;
private:
	bool LoadMeshToOpenGL();
	void UnloadOpenGLData();
};

class CModel
{
public:
	CModel();

	bool LoadModel(const std::string& Filename);
	const std::vector<CMesh>& GetMeshes();

private:
	std::vector<TextureStruct> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLuint LoadTextureFromFile(const char * path, std::string directory);
	std::vector<CMesh> m_meshes;
	std::vector<TextureStruct> textures_loaded;
	std::string m_directory;
	unsigned int m_meshNumber;
};