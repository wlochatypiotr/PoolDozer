#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "vertexData.h"
#include "GL/glew.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

struct RenderingData
{
	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
	unsigned int numIndices;
	unsigned int numVertices;
};

class OBJModel
{
public:
	OBJModel(const char * filename, std::vector<glm::vec4> &vertices, std::vector<glm::vec3> &normals, std::vector<GLushort> &elements);

	RenderingData m_renderingData;
};