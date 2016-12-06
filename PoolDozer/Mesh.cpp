#include "Mesh.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
}

bool CMesh::LoadMesh(const std::string & Filename)
{
	// Release the previously loaded mesh (if it exists)
	//Clear();

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (pScene) {
		Ret = InitFromScene(pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}

	return Ret;
}

bool CMesh::InitFromScene(const aiScene * pScene, const std::string & Filename)
{
	m_meshes.resize(pScene->mNumMeshes);
	m_textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_meshes.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return true;//InitMaterials(pScene, Filename);
}

void CMesh::InitMesh(unsigned int Index, const aiMesh * paiMesh)
{
	m_meshes[Index].materialIndex = paiMesh->mMaterialIndex;
	m_meshes[Index].numVertices = paiMesh->mNumVertices;
}
