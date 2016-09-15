#pragma once
#include <GL/glew.h>

struct VertexData
{
	//vertex position
	GLfloat x;
	GLfloat y;
	GLfloat z;

	//normals
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
	GLfloat nw;
};

struct RenderingData
{
	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
};

struct GLfloatx3
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};