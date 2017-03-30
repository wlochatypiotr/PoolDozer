#include "Model.h"

CModel::CModel()
{
}

bool CModel::LoadModel(const std::string & PathToFile)
{
	this->m_directory = PathToFile.substr(0, PathToFile.find_last_of('/'));
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(PathToFile.c_str(), aiProcess_Triangulate | 
																aiProcess_GenSmoothNormals | 
																aiProcess_JoinIdenticalVertices);
	bool ret;
	if (pScene) {
		//read mesh data
		m_meshNumber = pScene->mNumMeshes;
		m_meshes.resize(m_meshNumber);

		//load every mesh form file
		for (unsigned int i = 0; i < m_meshNumber; ++i)
		{
			aiMesh* currMesh = pScene->mMeshes[i];

			unsigned int vertNumber = currMesh->mNumVertices;
			std::vector<unsigned int> indices; //indices

			std::vector<vertexData> currMeshData; //vertices
			currMeshData.resize(vertNumber);

			//load position
			for (unsigned int j = 0; j < vertNumber; ++j)
			{
				aiVector3D pos = currMesh->mVertices[j];
				currMeshData[j].x = pos.x;
				currMeshData[j].y = pos.y;
				currMeshData[j].z = pos.z;

				//load normals
				if (currMesh->HasNormals())
				{
					aiVector3D norm = currMesh->mNormals[j];

					currMeshData[j].nx = norm.x;
					currMeshData[j].ny = norm.y;
					currMeshData[j].nz = norm.z;
				}
				//load texture coordinates
				if (currMesh->mTextureCoords[0])
				{
					currMeshData[j].tx = currMesh->mTextureCoords[0][j].x;
					currMeshData[j].ty = currMesh->mTextureCoords[0][j].y;
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
			}
			//load materials
			std::vector<TextureStruct> meshTextures;
			if (currMesh->mMaterialIndex > 0)
			{
				aiMaterial* material = pScene->mMaterials[currMesh->mMaterialIndex];

				//todo: extract default material from mesh

				//aiColor4D diffuse;
				//aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
				//color4<float> color = color4<float>(1.0f, 1.0f, 1.0f, 1.0f);
				//aiColor4D diffuse;
				//if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
				//	color = color4<float>(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
				//colors.push_back(color);

				// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
				// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
				// Same applies to other texture as the following list summarizes:
				// Diffuse: texture_diffuseN
				// Specular: texture_specularN
				// Normal: texture_normalN
				//material->mProperties->
				// 1. Diffuse maps
				std::vector<TextureStruct> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				meshTextures.insert(meshTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
				// 2. Specular maps
				std::vector<TextureStruct> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
				meshTextures.insert(meshTextures.end(), specularMaps.begin(), specularMaps.end());
				// 3. Normal maps
				std::vector<TextureStruct> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
				meshTextures.insert(meshTextures.end(), normalMaps.begin(), normalMaps.end());
			}

			m_meshes.at(i).m_indices = indices;
			m_meshes.at(i).m_vertices = currMeshData;
			m_meshes.at(i).m_textures = meshTextures;
			m_meshes.at(i).m_mesh.numVertices = vertNumber;
			m_meshes.at(i).m_mesh.numIndices = indices.size();

			//laod data to OpenGL
			ret = m_meshes.at(i).LoadMeshToOpenGL();

			if (!ret)
				std::cout << "mesh no. " << i << "in model " << this->m_directory << "could not be loaded" << std::endl;
		}
	}
	else {
		printf("Error parsing '%s': '%s'\n", PathToFile.c_str(), Importer.GetErrorString());
		ret = false;
	}

	return ret;
}


const std::vector<CMesh>& CModel::GetMeshes()
{
	return m_meshes;
}

std::vector<TextureStruct> CModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<TextureStruct> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			TextureStruct texture;
			texture.id = LoadTextureFromFile(str.C_Str(), this->m_directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

GLuint CModel::LoadTextureFromFile(const char * path, std::string directory)
{
	//Generate texture ID and load texture data 
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(filename.c_str(), 0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename.c_str());
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		std::cout << "unknown file type" << std::endl;

	FIBITMAP* image = nullptr;
	if (FreeImage_FIFSupportsReading(fif))
		image = FreeImage_Load(fif, filename.c_str());

	if (!image)
		std::cout << "Texture " << filename << "loading failed" << std::endl;

	image = FreeImage_ConvertTo24Bits(image);
	auto bits = FreeImage_GetBits(image);
	unsigned int width, height;
	width = FreeImage_GetWidth(image);
	height = FreeImage_GetHeight(image);

	if ((bits == 0) || (width == 0) || (height == 0))
		std::cout << "Texture loading somehow failed :P" << std::endl;

	//FreeImage_Save(fif, image, "test");
	auto dis = FreeImage_GetBPP(image);
	std::cout << "Texture has RGB format" << dis <<std::endl;
	GLuint textureID;
	glGenTextures(1, &textureID);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);

	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	FreeImage_Unload(image);
	return textureID;
}

bool CMesh::LoadMeshToOpenGL()
{
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData) * this->m_mesh.numVertices, this->m_vertices.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->m_mesh.numIndices, this->m_indices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), (GLvoid*)(5 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	this->m_mesh.EBO = EBO;
	this->m_mesh.VBO = VBO;
	this->m_mesh.VAO = VAO;

	return glGetError() == GL_NO_ERROR;
}

void CMesh::UnloadOpenGLData()
{
	glDeleteVertexArrays(1, &m_mesh.VAO);
	glDeleteBuffers(1, &m_mesh.VBO);
	glDeleteBuffers(1, &m_mesh.EBO);

	for (auto& c : m_textures)
	{
		glDeleteTextures(1, &c.id);
	}
}
