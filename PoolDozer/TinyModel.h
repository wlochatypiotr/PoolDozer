#pragma once
#include "tiny_obj_loader.h"
#include <iostream>
#include "GL/glew.h"
#include "Configuration.h"
#include "ShaderLoader.h"
#include "vertexData.h"

struct RenderingData
{
	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
	unsigned int numIndices;
	unsigned int numVertices;
};

std::vector<RenderingData> Load(const char* objFilename);




class TinyModel
{
public:
	TinyModel(const char *filename, Shader& program, glm::mat4 * model, glm::mat4  * view, glm::mat4 * projection, glm::vec3 * color, glm::vec3 * lightColor, glm::vec3 * lightPosition);
	TinyModel(const char *filename);
	TinyModel(const TinyModel&) = delete;
	TinyModel& operator= (const TinyModel&) = delete;
	~TinyModel();

	void Draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &color, const glm::vec3 &lightcolor);
	void Draw();

	tinyobj::attrib_t m_attrib;
	std::vector<tinyobj::shape_t> m_shapes;
	std::vector<tinyobj::material_t> m_materials;
	std::string m_err;

	//openGl data
	//GLuint m_VBO;
	//GLuint m_EBO;
	//GLuint m_VAO;
	unsigned m_numVertices;
	unsigned m_numIndices;

	std::vector<RenderingData> m_renderingData;

	Shader * m_program;

private:
	void LoadMesh();
	//matrices
	glm::mat4 * m_model = nullptr;
	glm::mat4 * m_view = nullptr;
	glm::mat4 * m_projection = nullptr;

	//vectors
	glm::vec3 * m_color = nullptr;
	glm::vec3 * m_lightColor = nullptr;
	glm::vec3 * m_lightPositon = nullptr;
};