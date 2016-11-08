#pragma once
#include "TinyModel.h"
class Drawable
{
public:
	Drawable(Shader& programName, GLfloat* vertices, GLuint numbVerts, GLuint* indices, GLuint numbIndices);
	Drawable(Shader& programName, GLfloat* vertices, GLuint numbVerts);
	Drawable(const Drawable&) = delete;
	Drawable& operator= (const Drawable&) = delete;
	~Drawable();

	void Draw();
	GLuint GetProgram();
private:
	Shader* m_program;
	RenderingData m_renderingData;
	GLuint m_verticesNumber;
	GLuint m_indicesNumber;
	bool m_useEBO;
};