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

void Renderer::DrawMesh(CEntity * entity)
{
	CECVisualMesh * component;
	component = static_cast<CECVisualMesh*> (entity->GetEntityComponent("ECVisual"));
	//static_cast<CECVisualMesh*> (component);
	if (component != nullptr)
	{
		//construct model matrix
		Transform trans = entity->GetTransform();
		glm::mat4 model = mat4();
		
		model = glm::scale(model, trans.m_scale);
		model = glm::translate(model, trans.m_position);

		//apply rotation
		if (trans.m_rotation.x != 0)
		{
			model = glm::rotate(model, glm::radians(trans.m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		if (trans.m_rotation.y != 0)
		{
			model = glm::rotate(model, glm::radians(trans.m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (trans.m_rotation.z != 0)
		{
			model = glm::rotate(model, glm::radians(trans.m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		//model = glm::translate(model, trans.m_position);
		////apply scale
		//model = glm::scale(model, trans.m_scale);

		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::translate(model, vec3(0.0f, 1.0f, -5.0f));
		//model = glm::rotate(model, glm::radians(95.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		if (component->m_program != nullptr)
			glUseProgram(component->m_program->Program);
		else
			glUseProgram(m_program.Program);

		//hardcoede matrix uniform location
		glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(m_view));
		glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(m_projection));

		//glUniform3f(6, component->m_color.x, component->m_color.y, component->m_color.z);

		//glUniform3f(Configuration::m_vertexAttributes::LIGHT_COLOR, component->m_color.x, component->m_color.y, component->m_color.z);
		//light pos (cube)
		//glUniform3f(Configuration::m_vertexAttributes::LIGHT_SOURCE_POS, component->m_color.x, component->m_color.y, component->m_color.z);

		for (auto c : component->m_mesh)
		{
			glBindVertexArray(c.VAO);

			glDrawElements(GL_TRIANGLES, c.numIndices, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		glUseProgram(0);
	}

}
