#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "fbxsdk.h"
#include "Soil.h"
#include <string>
#include "vertexData.h"
#include "GL/glew.h"
#include "ShaderLoader.h"
//#include <unordered_map>
//#include <algorithm>


struct RenderingData
{
	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
	unsigned int numIndices;
	unsigned int numVertices;
};

//struct MeshData
//{
//	std::unordered_map<unsigned int, floatx3*> mControlPoints; // dynamic memory ,must be freed
//	std::vector<Triangle> mTriangles;
//	std::vector<PNTIWVertex> mVertices;
//	unsigned int mTriangleCount;
//	GLuint VBO;
//	GLuint EBO;
//	GLuint VAO;
//	unsigned int numIndices;
//};
class FBXModel
{
public:
	FBXModel(const std::string &filePath, ShaderLoader& programName);
	FBXModel(const FBXModel&) = delete;
	FBXModel& operator= (const FBXModel&) = delete;
	~FBXModel();
	void setProgram(ShaderLoader& shaderName);
//private:
	void LoadNode(FbxNode * fbxNode);
	void LoadMesh(FbxMesh * fbxMesh);

	void LoadUVs(FbxMesh* fbxMesh, vertexData * VertexData);
	void LoadNormals(FbxMesh* fbxMesh, vertexData * VertexData);
	void LoadTextures(FbxNode* fbxNode);

	//nowe metody
	//void ProcessGeometry(FbxNode * inNode);	//odpowiednik load node
	////void ProcessControlPoints(FbxNode* inNode);
	//void ProcessMesh(FbxNode* inNode);
	//void AssociateMaterialToMesh(FbxNode* inNode);
	//void ProcessMaterials(FbxNode* inNode);
	////void ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, floatx3& outNormal);
	//void ReadUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, floatx2& outUV);
	////void Optimize();
	//int  FindVertex(const PNTIWVertex& inTargetVertex, const std::vector<PNTIWVertex>& uniqueVertices);
	void LoadScene();
	void Draw();
	void Draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &color, const glm::vec3 &lightcolor);

	const char* m_fileName;
	FbxManager * m_fbxManager;
	FbxGeometryConverter * m_fbxGeometryConverter;
	FbxScene* m_fbxScene;
	FbxIOSettings* m_fbxIOSettings;
	int m_numbVerts = 0;
	std::vector<RenderingData> m_renderingData;
//	std::vector<MeshData> m_meshData;
	
	ShaderLoader * m_program;

};