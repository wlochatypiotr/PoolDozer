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
	glUniformMatrix4fv(EUniformEnum::MODEL_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(mesh->m_model));
	glUniformMatrix4fv(EUniformEnum::VIEW_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(m_view));
	glUniformMatrix4fv(EUniformEnum::PROJECTION_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(m_projection));

	glUniform3f(EUniformEnum::OBJECT_COLOR_VEC3, mesh->m_color.x, mesh->m_color.y, mesh->m_color.z);

	//this uniform have use for light source
	//glUniform3f(EUniformEnum::LIGHT_COLOR_VEC3, component->m_color.x, component->m_color.y, component->m_color.z);
	
	//this uniform have use for object lightning
	//glUniform3f(EUniformEnum::LIGHT_POS_VEC3, component->m_color.x, component->m_color.y, component->m_color.z);

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
