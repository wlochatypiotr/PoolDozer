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

void Renderer::SetViewMatrix(const mat4& view)
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



void Renderer::DrawComponent(CECVisualMesh * mesh)
{
	if (mesh->m_program != nullptr)
		glUseProgram(mesh->m_program->Program);
	else
		glUseProgram(m_program.Program);

	//hardcoede matrix uniform location
	glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(mesh->m_model));
	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(m_view));
	glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(m_projection));

	//glUniform3f(6, component->m_color.x, component->m_color.y, component->m_color.z);

	//glUniform3f(Configuration::m_vertexAttributes::LIGHT_COLOR, component->m_color.x, component->m_color.y, component->m_color.z);
	//light pos (cube)
	//glUniform3f(Configuration::m_vertexAttributes::LIGHT_SOURCE_POS, component->m_color.x, component->m_color.y, component->m_color.z);

	for (auto c : mesh->m_mesh)
	{
		glBindVertexArray(c.VAO);

		glDrawElements(GL_TRIANGLES, c.numIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	glUseProgram(0);
}


void Renderer::Draw(CEntity * entity)
{
	CEntityComponent * entityComponent = entity->GetEntityComponent(entity_component_id_t("ECVisual"));
	if (entityComponent != nullptr)
	{
		entity_component_id_t componentID = entityComponent->ComponentID();

		if (componentID == "ECVisualMesh")
		{
			//call draw mesh
			CECVisualMesh * mesh = static_cast<CECVisualMesh*> (entityComponent);
			DrawComponent(mesh);
		}
		else
		{
			if (componentID == "ECVisualSphere")//draw sphere
			{
				CECVisualSphere * sphere = static_cast<CECVisualSphere*>(entityComponent);
				//DrawComponent(sphere);
				//draw sphere
			}
		}
	}
	else
		std::cout << "This entity has no visual component!";

}
