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

	void DrawMesh(CEntity * entity);
private:
	mat4 m_view;
	mat4 m_projection;
	Shader m_program;
};