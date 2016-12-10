#pragma once
#include "glm/gtc/type_ptr.hpp"
#include "Entity.h"

using namespace glm;

class Renderer
{
public:
	Renderer();
	enum EAttributeEnum
	{
		VERTEX_POS = 0,
		VERTEX_NORMAL = 1,
		TEXTURE_UV = 2
	};
	enum EUniformEnum
	{
		MODEL_MATRIX_4X4 = 3,
		VIEW_MATRIX_4X4 = 4,
		PROJECTION_MATRIX_4X4 = 5,
		OBJECT_COLOR_VEC3 = 6,
		LIGHT_COLOR_VEC3 = 7,
		LIGHT_POS_VEC3 = 8
	};

	void Initialize(const mat4& view, const mat4& proj, const Shader& shader);
	void SetViewMatrix(const mat4& view);
	void SetProjectionMatrix(const mat4& proj);
	void SetProgram(const Shader& shader);


	void Draw(CEntity * entity);
	void ClearBuffer();
private:
	//overloads on DrawComponent
	void DrawComponent(CECVisualMesh* mesh);
	mat4 m_view;// = nullptr;
	mat4 m_projection;// = nullptr;
	Shader m_program;
};