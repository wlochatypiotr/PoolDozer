#include "Mesh.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	CleaBuffer();
}

bool CMesh::LoadMesh(const std::string & Filename)
{
	// Release the previously loaded mesh (if it exists)
	//Clear();

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	bool ret;
	if (pScene) {
		//read mesh data
		unsigned int meshNumber = pScene->mNumMeshes;
		m_meshes.resize(meshNumber);

		//load every mesh form file
		for (unsigned int i = 0; i < meshNumber; ++i)
		{
			aiMesh* currMesh = pScene->mMeshes[i];

			unsigned int vertNumber = currMesh->mNumVertices;
			std::vector<unsigned int> indices;
			m_meshes.at(i).numVertices = vertNumber;

			//vertexData * meshData = new vertexData[vertNumber];
			std::vector<vertexData> meshData;
			meshData.resize(vertNumber);

			//load position
			for (unsigned int j = 0; j < vertNumber; ++j)
			{
				aiVector3D pos = currMesh->mVertices[j];
				meshData[j].x = pos.x;
				meshData[j].y = pos.y;
				meshData[j].z = pos.z;

				//load normals
				if (currMesh->HasNormals())
				{
					aiVector3D norm = currMesh->mNormals[j];

					meshData[j].nx = norm.x;
					meshData[j].ny = norm.y;
					meshData[j].nz = norm.z;
				}
			}
			//load indices
			if (currMesh->HasFaces())
			{
				unsigned int numFaces = currMesh->mNumFaces;
				for (unsigned int j = 0; j < numFaces; ++j)
				{
					const aiFace & currFace = currMesh->mFaces[j];
					indices.push_back(currFace.mIndices[0]);
					indices.push_back(currFace.mIndices[1]);
					indices.push_back(currFace.mIndices[2]);
				}
				m_meshes.at(i).numIndices = indices.size();
			}

			//laod data to OpenGL
			GLuint VAO, VBO, EBO;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(meshData[0]) * m_meshes.at(i).numVertices, meshData.data(), GL_STATIC_DRAW);

			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_meshes.at(i).numIndices, indices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 9 * sizeof(GLfloat), (GLvoid*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 9 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glBindVertexArray(0);

			m_meshes.at(i).VBO = VBO;
			m_meshes.at(i).EBO = EBO;
			m_meshes.at(i).VAO = VAO;
			ret = true;
		}
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
		ret = false;
	}

	return ret;
}

void CMesh::CleaBuffer()
{
	for (auto c : m_meshes)
	{
		glDeleteVertexArrays(1, &c.VAO);
		glDeleteBuffers(1, &c.VBO);
		glDeleteBuffers(1, &c.EBO);
	}
}

const std::vector<MeshStruct>& CMesh::GetMeshData()
{
	return m_meshes;
}


