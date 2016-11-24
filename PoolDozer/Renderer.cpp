#include "Renderer.h"

Renderer::Renderer()
{
}

void Renderer::Initialize(const mat4& view, const mat4& proj, const Shader& shader)
{
	SetViewMatrix(view);
	SetProjectionMatrix(proj);
	SetProgram(shader);
}

void Renderer::SetViewMatrix(const mat4 & view)
{
	m_view = view;
}

void Renderer::SetProjectionMatrix(const mat4 & proj)
{
	m_projection = proj;
}

void Renderer::SetProgram(const Shader & shader)
{
	m_program = shader;
}

void Renderer::DrawMesh(CEntity * entity)
{
	CEntityComponent * component;
	component = entity->GetEntityComponent("ECVisual");
	if (component != nullptr)
	{

	}
}
