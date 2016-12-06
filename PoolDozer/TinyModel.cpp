#include "TinyModel.h"

TinyModel::TinyModel(const char * filename, Shader& program, glm::mat4 * model, glm::mat4  * view, glm::mat4 * projection, glm::vec3 * color, glm::vec3 * lightColor, glm::vec3 * lightPosition)
	: m_program(&program)
	, m_model(model)
	, m_view(view)
	, m_projection(projection)
	, m_color(color)
	, m_lightColor(lightColor)
	, m_lightPositon(lightPosition)
{
	if (tinyobj::LoadObj(&m_attrib, &m_shapes, &m_materials, &m_err, filename, NULL, true))
		std::cout << "Loading of model: " << filename << " is complete!" << std::endl << m_err;
	LoadMesh();
	
}

TinyModel::TinyModel(const char *filename)
{
	if (tinyobj::LoadObj(&m_attrib, &m_shapes, &m_materials, &m_err, filename, NULL, true))
		std::cout << "Loading of model: " << filename << " is complete!" << std::endl << m_err;
	LoadMesh();
}

TinyModel::~TinyModel()
{
	for (auto c : m_renderingData)
	{
		glDeleteVertexArrays(1, &c.VAO);
		glDeleteBuffers(1, &c.VBO);
		glDeleteBuffers(1, &c.EBO);
	}

}

void TinyModel::Draw(const glm::mat4 & model, const glm::mat4 & view, const glm::mat4 & projection, const glm::vec3 & color, const glm::vec3 & lightcolor)
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
		//glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
		glBindVertexArray(0);
	}

	glUseProgram(0);
}

void TinyModel::Draw()
{
	glUseProgram(m_program->Program);
	glUniformMatrix4fv(Configuration::m_vertexAttributes::MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(*m_model));
	glUniformMatrix4fv(Configuration::m_vertexAttributes::VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(*m_view));
	glUniformMatrix4fv(Configuration::m_vertexAttributes::PROJECTION_MATRIX, 1, GL_FALSE, glm::value_ptr(*m_projection));
	//color
	glUniform3f(Configuration::m_vertexAttributes::OBJECT_COLOR,m_color->x, m_color->y, m_color->z);
	//light color
	glUniform3f(Configuration::m_vertexAttributes::LIGHT_COLOR, m_lightColor->x, m_lightColor->y, m_lightColor->z);
	//light pos (cube)
	glUniform3f(Configuration::m_vertexAttributes::LIGHT_SOURCE_POS, m_lightPositon->x, m_lightPositon->y, m_lightPositon->z);

	for (auto c : m_renderingData)
	{
		glBindVertexArray(c.VAO);

		glDrawElements(GL_TRIANGLES, c.numIndices, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
		glBindVertexArray(0);
	}

	glUseProgram(0);
}

void TinyModel::LoadMesh()
{
	//calculate vertex number for model
	m_numVertices = m_attrib.vertices.size();

	//calculate index number for model
	for (size_t i = 0; i < m_shapes.size(); ++i)
	{
		m_numIndices += m_shapes[i].mesh.indices.size();
	}

	// all model vertices
	float* vertices = &m_attrib.vertices[0];
	// all model normals
	float* normals = &m_attrib.normals[0];

	//for each mesh in the model
	for (size_t i = 0; i < m_shapes.size(); ++i)
	{
		RenderingData meshData;
		meshData.numVertices = 0;
		meshData.numIndices = m_shapes[i].mesh.indices.size();
		// moze to mozna jakos usprawnic ?
		for (size_t j = 0; j < m_shapes[i].mesh.indices.size(); ++j)
		{
			if (meshData.numVertices < m_shapes[i].mesh.indices[j].vertex_index)
				meshData.numVertices = m_shapes[i].mesh.indices[j].vertex_index;

		}
		meshData.numVertices += 1;
		std::cout << "number of verts is: " << meshData.numVertices << std::endl;
		std::cout << "number of indices is: " << meshData.numIndices << std::endl;

		vertexData * currMesh = new vertexData[meshData.numVertices];

		unsigned int * indices = new unsigned int[meshData.numIndices];

		//std::copy(m_shapes[i].mesh.indices)
		for (size_t j = 0; j < meshData.numIndices; ++j)
		{
			indices[j] = m_shapes[i].mesh.indices[j].vertex_index;
		}

		//load mesh normals and mesh vertices
		for (size_t j = 0, vindex = 0, nindex = 0; j < m_shapes[i].mesh.indices.size(); ++j)
		{
			vindex = m_shapes[i].mesh.indices[j].vertex_index;
			nindex = m_shapes[i].mesh.indices[j].normal_index;

			//vertices
			currMesh[vindex].x = vertices[(vindex * 3)];
			currMesh[vindex].y = vertices[(vindex * 3) + 1];
			currMesh[vindex].z = vertices[(vindex * 3) + 2];

			//normals
			currMesh[vindex].nx = normals[(nindex * 3)];
			currMesh[vindex].ny = normals[(nindex * 3) + 1];
			currMesh[vindex].nz = normals[(nindex * 3) + 2];

			//texcords...
		}
		//send data to openGL
		GLuint VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(*currMesh) * meshData.numVertices, currMesh, GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * m_numVertices, vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData.numIndices, indices, GL_STATIC_DRAW);

		glVertexAttribPointer(Configuration::m_vertexAttributes::VERTEX_POSITION, 3, GL_FLOAT, GL_TRUE, 9 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(Configuration::m_vertexAttributes::VERTEX_NORMAL, 3, GL_FLOAT, GL_TRUE, 9 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		//glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)3);	//texture cordinates
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		meshData.VBO = std::move(VBO);
		meshData.EBO = std::move(EBO);
		meshData.VAO = std::move(VAO);
		m_renderingData.push_back(std::move(meshData));

		delete[] currMesh;
		delete[] indices;
	}
}

std::vector<RenderingData> Load(const char* objFilename)
{
	{
		std::vector<RenderingData> mesh;
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;
		unsigned int m_numVertices = 0;
		unsigned int m_numIndices = 0;

		if (tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objFilename, NULL, true))
			std::cout << "Loading of model: " << objFilename << " is complete!" << std::endl << err;


		//LoadMesh function here
		//calculate vertex number for model
		m_numVertices = attrib.vertices.size();

		//calculate index number for model
		for (size_t i = 0; i < shapes.size(); ++i)
		{
			m_numIndices += shapes[i].mesh.indices.size();
		}

		// all model vertices
		float* vertices = &attrib.vertices[0];
		// all model normals
		float* normals = &attrib.normals[0];

		//for each mesh in the model
		for (size_t i = 0; i < shapes.size(); ++i)
		{
			RenderingData meshData;
			meshData.numVertices = 0;
			meshData.numIndices = shapes[i].mesh.indices.size();
			// moze to mozna jakos usprawnic ?
			for (size_t j = 0; j < shapes[i].mesh.indices.size(); ++j)
			{
				if (meshData.numVertices < shapes[i].mesh.indices[j].vertex_index)
					meshData.numVertices = shapes[i].mesh.indices[j].vertex_index;

			}
			meshData.numVertices += 1;
			std::cout << "number of verts is: " << meshData.numVertices << std::endl;
			std::cout << "number of indices is: " << meshData.numIndices << std::endl;

			vertexData * currMesh = new vertexData[meshData.numVertices];

			unsigned int * indices = new unsigned int[meshData.numIndices];

			//std::copy(m_shapes[i].mesh.indices)
			for (size_t j = 0; j < meshData.numIndices; ++j)
			{
				indices[j] = shapes[i].mesh.indices[j].vertex_index;
			}

			//load mesh normals and mesh vertices
			for (size_t j = 0, vindex = 0, nindex = 0; j < shapes[i].mesh.indices.size(); ++j)
			{
				vindex = shapes[i].mesh.indices[j].vertex_index;
				nindex = shapes[i].mesh.indices[j].normal_index;

				//vertices
				currMesh[vindex].x = vertices[(vindex * 3)];
				currMesh[vindex].y = vertices[(vindex * 3) + 1];
				currMesh[vindex].z = vertices[(vindex * 3) + 2];

				//normals
				currMesh[vindex].nx = normals[(nindex * 3)];
				currMesh[vindex].ny = normals[(nindex * 3) + 1];
				currMesh[vindex].nz = normals[(nindex * 3) + 2];

				//texcords...
			}
			//send data to openGL
			GLuint VAO, VBO, EBO;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(*currMesh) * meshData.numVertices, currMesh, GL_STATIC_DRAW);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * m_numVertices, vertices, GL_STATIC_DRAW);

			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData.numIndices, indices, GL_STATIC_DRAW);

			glVertexAttribPointer(Configuration::m_vertexAttributes::VERTEX_POSITION, 3, GL_FLOAT, GL_TRUE, 9 * sizeof(GLfloat), (GLvoid*)0);
			glVertexAttribPointer(Configuration::m_vertexAttributes::VERTEX_NORMAL, 3, GL_FLOAT, GL_TRUE, 9 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
			//glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)3);	//texture cordinates
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glBindVertexArray(0);
			meshData.VBO = std::move(VBO);
			meshData.EBO = std::move(EBO);
			meshData.VAO = std::move(VAO);
			mesh.push_back(std::move(meshData));

			delete[] currMesh;
			delete[] indices;
		}
		return std::move(mesh);
	}
}
