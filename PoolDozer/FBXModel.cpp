#include "FBXModel.h"

FBXModel::FBXModel(const std::string & filePath, ShaderLoader& programName) : m_fileName(filePath.c_str())
{
	LoadScene();
	auto  matrix = m_fbxScene->GetRootNode()->EvaluateGlobalTransform();
	auto  matrixL = m_fbxScene->GetRootNode()->EvaluateLocalTransform();
	setProgram(programName);
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	matrix = m_fbxScene->GetRootNode()->EvaluateGlobalTransform();
	LoadNode(m_fbxScene->GetRootNode());

	/*FbxArray<FbxTexture*>  textures;
	scene->FillTextureArray(textures);
	std::cout << textures.GetCount(); */
	//ProcessGeometry(scene->GetRootNode());
	//for (auto c : meshData)
	//{
	//	std::cout << "Number of mVertices: " << c.mVertices.size() << "\n";
	//}
	//glBindVertexArray(0);

	/*for (auto c : renderingData)
	{
		std::cout << "Number of vertices in mesh: " << c.numVertices << "\n";
		std::cout << "Number of vertices indices in mesh: " << c.numIndices << "\n";
	}
*/
}

FBXModel::~FBXModel()
{
	m_fbxScene->Destroy();
	m_fbxIOSettings->Destroy();
	m_fbxManager->Destroy();
	for (auto c : m_renderingData)
	{ 
		glDeleteVertexArrays(1, &c.VAO);
		glDeleteBuffers(1, &c.VBO);
		glDeleteBuffers(1, &c.EBO);
	}
	//for (auto c : m_meshData)
	//{
	//	glDeleteVertexArrays(1, &c.VAO);
	//	glDeleteBuffers(1, &c.VBO);
	//	glDeleteBuffers(1, &c.EBO);
	//}
	//	glDeleteVertexArrays(1, &c);
	////glDeleteBuffers(1, &VBO);
	//for (auto c : vbos)
	//	glDeleteBuffers(1, &c);
	//for (auto c : ebos)
	//	glDeleteBuffers(1, &c);
}

void FBXModel::setProgram(ShaderLoader& shaderName)
{
	m_program = &shaderName;
}

void FBXModel::LoadNode(FbxNode * fbxNode)
{
	int numAttributes = fbxNode->GetNodeAttributeCount();
	for (int i = 0; i < numAttributes; i++)
	{
		FbxNodeAttribute *nodeAttributeFbx = fbxNode->GetNodeAttributeByIndex(i);
		FbxNodeAttribute::EType attributeType = nodeAttributeFbx->GetAttributeType();

		switch (attributeType)
		{
		case FbxNodeAttribute::eMesh:
		{
			// Load keyframe transformations
			//this->LoadNodeKeyframeAnimation(fbxNode);

			// Load mesh vertices, texcoords, normals, etc
			this->LoadMesh((FbxMesh*)nodeAttributeFbx);

			//load textures
			LoadTextures(fbxNode);
			
			// Load mesh skeleton
			//this->LoadMesh_Skeleton((FbxMesh*)nodeAttributeFbx);
			break;
		}
		}
	}

	// Load the child nodes
	int numChildren = fbxNode->GetChildCount();
	for (int i = 0; i < numChildren; i++)
	{
		this->LoadNode(fbxNode->GetChild(i));
	}
}

void FBXModel::LoadMesh(FbxMesh * fbxMesh)
{
	// Load Vertices
	FbxMatrix globalTransform = fbxMesh->GetNode()->EvaluateGlobalTransform();;
	m_numbVerts += fbxMesh->GetControlPointsCount();
	const int numVertices = fbxMesh->GetControlPointsCount();
	auto vertices =  new vertexData[numVertices];
	FbxVector4 *verticesFbx = fbxMesh->GetControlPoints();
	//fbxsdk::FbxArray<FbxVector2> UVs;
	//fbxMesh->GetPolygonVertexUVs("map1", UVs);
	int index = 0;
	for (int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
	{
		(float)vertices[index].x = (float)verticesFbx[vertexIndex][0];
		(float)vertices[index].y = (float)verticesFbx[vertexIndex][1];
		(float)vertices[index].z = (float)verticesFbx[vertexIndex][2];
		
		++index;
	}
	//Load indices
	RenderingData meshData;
	meshData.numVertices = std::move(numVertices);
	meshData.numIndices = fbxMesh->GetPolygonVertexCount();
	int *indices = fbxMesh->GetPolygonVertices();

	//////Load texture cords 
	LoadUVs(fbxMesh, vertices);

	//Load Normals
	LoadNormals(fbxMesh, vertices);
	//Textures
	
	//for (int i = 0; i < fbxMesh->GetControlPointsCount(); ++i)
	//{
	//	std::cout << "UV value x : " << vertices[i].tx << " Uv value y: " << vertices[i].ty << "\n";
	//}

	//Prepare data for OpenGL
	GLuint VAO,VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * numVertices, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData.numIndices, indices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(Configuration::m_vertexAttributes::VERTEX_POSITION, 3, GL_FLOAT, GL_TRUE, 9 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(Configuration::m_vertexAttributes::VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	//glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)3);	//texture cordinates
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	meshData.VBO = std::move(VBO);
	meshData.EBO = std::move(EBO);
	meshData.VAO = std::move(VAO);
	//meshData.numIndices = numIndices;
	m_renderingData.push_back(std::move(meshData));
	//delete indices;
	delete[] vertices;
}

void FBXModel::LoadUVs(FbxMesh * fbxMesh, vertexData* VertexData)
{
	FbxStringList UVSetNameList;
	fbxMesh->GetUVSetNames(UVSetNameList);
	int UVSetCount = UVSetNameList.GetCount();
//	std::cout << "UVSetcount: " << UVSetCount << "\n";	//helper
	for (int UVSetIndex = 0; UVSetIndex < UVSetCount; UVSetIndex++)
	{
		const char * UVSetName = UVSetNameList.GetStringAt(UVSetIndex);
		//std::cout<< "Current set name: " <<  UVSetName << "\n";		//helper
		FbxGeometryElementUV* UVElement = fbxMesh->GetElementUV(UVSetName);

		if (! UVElement)
			continue;
		UVElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
		int IndexCount = UVElement->GetDirectArray().GetCount();
		//std::cout << "Index Count: " << IndexCount << "\n";
		for (int vertexIndex = 0; vertexIndex < fbxMesh->GetControlPointsCount(); ++vertexIndex)
		{
			FbxVector2 UVs;
			UVs = UVElement->GetDirectArray().GetAt(vertexIndex);
			VertexData[vertexIndex].tx = UVs.mData[0];
			VertexData[vertexIndex].ty = UVs.mData[1];
		}
	}

}

void FBXModel::LoadNormals(FbxMesh * fbxMesh, vertexData * VertexData)
{
	FbxGeometryElementNormal* meshNormal = fbxMesh->GetElementNormal();
	meshNormal->SetReferenceMode(FbxGeometryElementNormal::eDirect);
	meshNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);
	for (int vertexIndex = 0; vertexIndex != fbxMesh->GetControlPointsCount(); ++vertexIndex)
	{
		int normalIndex = 0;
		if (meshNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIndex = vertexIndex;
		if (meshNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			normalIndex = meshNormal->GetIndexArray().GetAt(vertexIndex);
		FbxVector4 normal = meshNormal->GetDirectArray().GetAt(normalIndex);

		VertexData[vertexIndex].nx = normal[0];
		VertexData[vertexIndex].ny = normal[1];
		VertexData[vertexIndex].nz = normal[2];
		VertexData[vertexIndex].nw = normal[3];
	}
}

void FBXModel::LoadTextures(FbxNode * fbxNode)
{

	unsigned int materialCount = fbxNode->GetMaterialCount();
	std::cout << "Material count:" << materialCount << "\n";

	const char * textureName = nullptr;
	for (unsigned int i = 0; i < materialCount; ++i)
	{
		 FbxSurfaceMaterial* surfaceMaterial = fbxNode->GetMaterial(i);
		 if (surfaceMaterial)
		 {
			 std::cout << "We have a surface Material. Material name is: " << surfaceMaterial->GetName() << "\n";
		 }
		unsigned int textureIndex = 0;
		FbxProperty property;
		
		property = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
		if (property.IsValid())
		{
			std::cout << "Our material is diffuse!\n";
			//check if it's a layered texture
			int layeredTextureCount = property.GetSrcObjectCount<FbxLayeredTexture>();
			if (layeredTextureCount > 0)
			{
				std::cout << "Layered Texture count is: " << layeredTextureCount << "\n";
			}
			else
			{
				int textureCount = property.GetSrcObjectCount<FbxTexture>();
				std::cout << "Our Texture count is: " << textureCount << "\n";
			}
		}
	
		
		FBXSDK_FOR_EACH_TEXTURE(textureIndex)
		{
			property = surfaceMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
			if (property.IsValid())
			{
				//std::cout << "Property is valid!\n";
				//std::cout << "property object count" << property.GetSrcObjectCount() << "\n";
				unsigned int textureCount = property.GetSrcObjectCount<FbxTexture>();
				//std::cout << "Texture count:" << textureCount << "zlo\n";
				for (unsigned int j = 0; j < textureCount; ++j)
				{
					FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(j);
					FbxProceduralTexture* proceduralTexture = property.GetSrcObject<FbxProceduralTexture>(j);
					if (proceduralTexture)
					{
						std::cout << "We have a procedural texture!\n";
					}
					if (layeredTexture)
					{
						throw std::exception("Layered Texture is currently unsupported\n");
					}
					else
					{
						FbxTexture* texture = property.GetSrcObject<FbxTexture>(j); //our texture
						if (texture)
						{
							//texture->GetS
							std::string textureType = property.GetNameAsCStr();
							std::cout <<"We have a texture: " << textureType <<  "\n";
							FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
							//surfaceMaterial->FindSrcObject(fileTexture);

							if (fileTexture)
							{
								if (textureType == "DiffuseColor")
								{
									textureName = fileTexture->GetFileName();
								}
								else if (textureType == "SpecularColor")
								{
									textureName = fileTexture->GetFileName();
								}
								else if (textureType == "Bump")
								{
									textureName = fileTexture->GetFileName();
								}
								auto texture = surfaceMaterial->FindSrcObject(textureName);
								
								std::cout << "Texture name: " << textureName << "\n";
								int width, height;
								unsigned char* image = SOIL_load_image(textureName, &width, &height, 0, SOIL_LOAD_RGB);
								std::cout << "texture size: " << sizeof(image) << "\n";
								//texture->ConnectSrcObject()
								//auto data = texture->GetUserDataPtr();
								//std::cout << "Sizeof texture: " << sizeof(data) << "\n";
							}
						}
					}
				}
			}
		}


	}
}
//
//void FBXModel::ProcessGeometry(FbxNode * inNode)
//{
//	if (inNode->GetNodeAttribute())
//	{
//		switch (inNode->GetNodeAttribute()->GetAttributeType())
//		{
//		case FbxNodeAttribute::eMesh:
//			ProcessControlPoints(inNode);
//			/*if (mHasAnimation)
//			{
//				ProcessJointsAndAnimations(inNode);
//			}*/
//			//ProcessMesh(inNode);
//			//AssociateMaterialToMesh(inNode);
//			//ProcessMaterials(inNode);
//			break;
//		}
//	}
//
//	for (int i = 0; i < inNode->GetChildCount(); ++i)
//	{
//		ProcessGeometry(inNode->GetChild(i));
//	}
//}
//
//void FBXModel::ProcessControlPoints(FbxNode * inNode)
//{
//	FbxMesh* currMesh = inNode->GetMesh();
//	MeshData ourMesh;
//	unsigned int ctrlPointCount = currMesh->GetControlPointsCount();
//	for (unsigned int i = 0; i < ctrlPointCount; ++i)
//	{
//		floatx3* currCtrlPoint = new floatx3;
//		floatx3 currPosition;
//		currPosition.x = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
//		currPosition.y = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
//		currPosition.z = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);
//		*currCtrlPoint = currPosition;
//		ourMesh.mControlPoints[i] = currCtrlPoint;
//	}
//
//	//process Mesh function
//
//	ourMesh.mTriangleCount = currMesh->GetPolygonCount();
//	int vertexCounter = 0;
//	ourMesh.mTriangles.reserve(ourMesh.mTriangleCount);
//
//	for (unsigned int i = 0; i < ourMesh.mTriangleCount; ++i)
//	{
//		floatx3 normal[3];
//		floatx3 tangent[3];
//		floatx3 binormal[3];
//		floatx2 UV[3][2];
//		Triangle currTriangle;
//		ourMesh.mTriangles.push_back(currTriangle);
//
//		for (unsigned int j = 0; j < 3; ++j)
//		{
//			int ctrlPointIndex = currMesh->GetPolygonVertex(i, j);
//			floatx3* currCtrlPoint = ourMesh.mControlPoints[ctrlPointIndex];
//
//			//ReadNormal(currMesh, ctrlPointIndex, vertexCounter, normal[j]);
//
//			for (int k = 0; k < 1; ++k)
//			{
//				ReadUV(currMesh, ctrlPointIndex, currMesh->GetTextureUVIndex(i, j), k, UV[j][k]);
//			}
//
//			PNTIWVertex temp;
//			temp.mPosition = *currCtrlPoint;
//			temp.mNormal = normal[j];
//			temp.mUV = UV[j][0];
//
//			ourMesh.mVertices.push_back(temp);
//			ourMesh.mTriangles.back().mIndices.push_back(vertexCounter);
//			++vertexCounter;
//		}
//	}
//	for (auto itr = ourMesh.mControlPoints.begin(); itr != ourMesh.mControlPoints.end(); ++itr)
//	{
//		delete itr->second;
//	}
//	ourMesh.mControlPoints.clear();
//	//load vertex Indices
//	ourMesh.numIndices = currMesh->GetPolygonVertexCount();
//	int *indices = currMesh->GetPolygonVertices();
//
//
//	//optimize, delete duplicated vertices
//	std::vector<PNTIWVertex> uniqueVertices;
//	for (unsigned int i = 0; i < ourMesh.mTriangles.size(); ++i)
//	{
//		for (unsigned int j = 0; j < 3; ++j)
//		{
//			// If current vertex has not been added to
//			// our unique vertex list, then we add it
//			if (FindVertex(ourMesh.mVertices[i * 3 + j], uniqueVertices) == -1)
//			{
//				uniqueVertices.push_back(ourMesh.mVertices[i * 3 + j]);
//			}
//		}
//	}
//		// Now we update the index buffer
//		for (unsigned int i = 0; i < ourMesh.mTriangles.size(); ++i)
//		{
//			for (unsigned int j = 0; j < 3; ++j)
//			{
//				ourMesh.mTriangles[i].mIndices[j] = FindVertex(ourMesh.mVertices[i * 3 + j], uniqueVertices);
//			}
//		}
//
//		ourMesh.mVertices.clear();
//		ourMesh.mVertices = uniqueVertices;
//		uniqueVertices.clear();
//
//		// Now we sort the triangles by materials to reduce 
//		// shader's workload
//		std::sort(ourMesh.mTriangles.begin(), ourMesh.mTriangles.end());
//	
//	
//	////////////////////////////////////////
//		//opengl data
//		//Prepare data for OpenGL
//		GLuint VAO, VBO, EBO;
//		glGenVertexArrays(1, &VAO);
//		glBindVertexArray(VAO);
//		glGenBuffers(1, &VBO);
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//		glBufferData(GL_ARRAY_BUFFER, 3 *sizeof(float) * ourMesh.mVertices.size(), &ourMesh.mVertices[0],GL_STATIC_DRAW);
//
//		glGenBuffers(1, &EBO);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ourMesh.numIndices, indices, GL_STATIC_DRAW);
//
//
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
//		glEnableVertexAttribArray(0);
//
//		glBindVertexArray(0);
//		ourMesh.VBO = std::move(VBO);
//		ourMesh.EBO = std::move(EBO);
//		ourMesh.VAO = std::move(VAO);
//	m_meshData.push_back(ourMesh);
//}
//
//void FBXModel::ProcessMesh(FbxNode * inNode)
//{
//}
//
//void FBXModel::AssociateMaterialToMesh(FbxNode * inNode)
//{
//}
//
//void FBXModel::ProcessMaterials(FbxNode * inNode)
//{
//}
//
//void FBXModel::ReadUV(FbxMesh * inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, floatx2 & outUV)
//{
//	if (inUVLayer >= 2 || inMesh->GetElementUVCount() <= inUVLayer)
//	{
//		throw std::exception("Invalid UV Layer Number");
//	}
//	FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(inUVLayer);
//
//	switch (vertexUV->GetMappingMode())
//	{
//	case FbxGeometryElement::eByControlPoint:
//		switch (vertexUV->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
//			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
//		}
//		break;
//
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
//			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
//			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
//		}
//		break;
//
//		default:
//			throw std::exception("Invalid Reference");
//		}
//		break;
//
//	case FbxGeometryElement::eByPolygonVertex:
//		switch (vertexUV->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
//			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
//		}
//		break;
//
//		default:
//			throw std::exception("Invalid Reference");
//		}
//		break;
//	}
//}
//
//int FBXModel::FindVertex(const PNTIWVertex & inTargetVertex, const std::vector<PNTIWVertex>& uniqueVertices)
//{
//	for (unsigned int i = 0; i < uniqueVertices.size(); ++i)
//	{
//		if (inTargetVertex == uniqueVertices[i])
//		{
//			return i;
//		}
//	}
//
//	return -1;
//}

void FBXModel::LoadScene()
{
	m_fbxManager = FbxManager::Create();
	m_fbxGeometryConverter = &FbxGeometryConverter(m_fbxManager);
	m_fbxIOSettings = FbxIOSettings::Create(m_fbxManager, IOSROOT);
	FbxImporter* lImporter = FbxImporter::Create(m_fbxManager, "");
	if (!lImporter->Initialize(m_fileName, -1, m_fbxManager->GetIOSettings()))
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}
	// Create a new scene so that it can be populated by the imported file.
	m_fbxScene = FbxScene::Create(m_fbxManager, "myScene");

	m_fbxGeometryConverter->SplitMeshesPerMaterial(m_fbxScene, true);
	if (m_fbxGeometryConverter->SplitMeshesPerMaterial(m_fbxScene, true))
		std::cout << "Mesh conversion complete ! \n";


	// Import the contents of the file into the scene.
	lImporter->Import(m_fbxScene);

	///////////////////////////////////////
	//int up;
	//auto curr = m_fbxScene->GetGlobalSettings().GetAxisSystem().GetUpVector(up);

	//FbxAxisSystem axisSystem = FbxAxisSystem::OpenGL;
	//axisSystem.ConvertScene(m_fbxScene);
	//////////////////////////////////////////////

	// The file is imported, so get rid of the importer.
	//cene->FillTextureArray()
	fbxsdk::FbxArray<FbxTexture*> textures;
	m_fbxScene->FillTextureArray(textures);
	
}

void FBXModel::Draw()
{
	m_program->Use();
	for (auto c : m_renderingData)
	{
		glBindVertexArray(c.VAO);

		glDrawElements(GL_LINES, c.numIndices,GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_POINTS, 0, c.numVertices);
		glBindVertexArray(0);
	}
}

void FBXModel::Draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &color, const glm::vec3 &lightcolor)
{
	glUseProgram(m_program->Program);
	glUniformMatrix4fv(Configuration::m_vertexAttributes::MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(Configuration::m_vertexAttributes::VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(Configuration::m_vertexAttributes::PROJECTION_MATRIX, 1, GL_FALSE, glm::value_ptr(projection));
	//color
	glUniform3f(Configuration::m_vertexAttributes::OBJECT_COLOR, color.x, color.y, color.z);
	//light color
	glUniform3f(Configuration::m_vertexAttributes::LIGHT_COLOR, lightcolor.x, lightcolor.y, lightcolor.z);
	//light pos (cube)
	glUniform3f(Configuration::m_vertexAttributes::LIGHT_SOURCE_POS, Configuration::m_lightPosition.x, Configuration::m_lightPosition.y, Configuration::m_lightPosition.z);

	for (auto c : m_renderingData)
	{
		glBindVertexArray(c.VAO);

		glDrawElements(GL_TRIANGLES, c.numIndices, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_POINTS, 0, c.numVertices);
		glBindVertexArray(0);
	}
	
	glUseProgram(0);
}
