#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ShaderLoader.h"
#include "Entity.h"

using namespace glm;

class Renderer
{
public:
	Renderer();


	void Initialize(const mat4& view, const mat4& proj, const Shader& shader);
	void SetViewMatrix(const mat4& view);
	void SetProjectionMatrix(const mat4& proj);
	void SetProgram(const Shader& shader);

	//void DrawMesh(CECVisualMesh* mesh);
	void Draw(CEntity * entity);
private:
	//overloads on DrawComponent
	void DrawComponent(CECVisualMesh* mesh);
	mat4 m_view;// = nullptr;
	mat4 m_projection;// = nullptr;
	Shader m_program;
};