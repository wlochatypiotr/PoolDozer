#include "Drawable.h"

Drawable::Drawable(ShaderLoader& programName, GLfloat* vertices, GLuint numbVerts, GLuint* indices, GLuint numbIndices) :m_verticesNumber(numbVerts), m_indicesNumber(numbIndices), m_useEBO(true)
{
	m_program = &programName;
	//set up data for rendering
	glGenVertexArrays(1, &m_renderingData.VAO);
	glGenBuffers(1, &m_renderingData.VBO);
	glGenBuffers(1, &m_renderingData.EBO);

	glBindVertexArray(m_renderingData.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_renderingData.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * m_verticesNumber, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderingData.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*indices)* m_indicesNumber, indices, GL_STATIC_DRAW);
	//m_useEBO = true;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

}

Drawable::Drawable(ShaderLoader & programName, GLfloat* vertices, GLuint numbVerts) : m_verticesNumber(numbVerts), m_useEBO(false)
{
	m_program = &programName;
	//set up data for rendering
	glGenVertexArrays(1, &m_renderingData.VAO);
	glGenBuffers(1, &m_renderingData.VBO);


	glBindVertexArray(m_renderingData.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_renderingData.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * m_verticesNumber, vertices, GL_STATIC_DRAW);
	m_useEBO = false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

Drawable::~Drawable()
{
	glDeleteVertexArrays(1, &m_renderingData.VAO);
	glDeleteBuffers(1, &m_renderingData.VBO);
	glDeleteBuffers(1, &m_renderingData.EBO);
}

void Drawable::Draw()
{

	glUseProgram(GetProgram());
	glBindVertexArray(m_renderingData.VAO);
	if (m_useEBO = true)
	{
		glDrawElements(GL_TRIANGLES, m_indicesNumber, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_verticesNumber);
	}
	glBindVertexArray(0);
	glUseProgram(0);
}

GLuint Drawable::GetProgram()
{
	return m_program->Program;
}
