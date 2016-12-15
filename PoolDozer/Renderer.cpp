#include "Renderer.h"

Renderer::Renderer()
{
}

void Renderer::Initialize(const Shader& shader, CWindowManager * mgr)
{
	//compute view matrix
	m_windowManager = mgr;

	m_view = glm::mat4();
	m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, -2.6f));
	m_view = glm::rotate(m_view, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	
	//compute projection matrix
	m_projection = glm::mat4();
	m_projection = glm::perspective(glm::radians(45.0f), (GLfloat)m_windowManager->GetWindowWidth() / (GLfloat)m_windowManager->GetWindowHeight(), 0.1f, 100.0f);

	//SetViewMatrix(view);
	//SetProjectionMatrix(proj);

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



void Renderer::DrawComponent(CECVisualMesh * meshComponent)
{
	if (meshComponent->m_program != nullptr)
		glUseProgram(meshComponent->m_program->Program);
	else
		glUseProgram(m_program.Program);

	//hardcoede matrix uniform location
	glUniformMatrix4fv(EUniformEnum::MODEL_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(meshComponent->m_model));
	glUniformMatrix4fv(EUniformEnum::VIEW_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(m_view));
	glUniformMatrix4fv(EUniformEnum::PROJECTION_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(m_projection));

	glUniform3f(EUniformEnum::OBJECT_COLOR_VEC3, meshComponent->m_color.x, meshComponent->m_color.y, meshComponent->m_color.z);

	//this uniform have use for light source
	//hardcoded light value
	glUniform3f(EUniformEnum::LIGHT_COLOR_VEC3, 1.0f, 1.0f, 1.0f);
	
	//this uniform have use for object lightning
	//hardcoded light position
	glUniform3f(EUniformEnum::LIGHT_POS_VEC3, 0.0f,1.0f, -5.0f);


	for (MeshStruct c : meshComponent->GetMesh()->GetMeshData())
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

void Renderer::ClearBuffer()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_windowManager->GetWindowWidth(), m_windowManager->GetWindowHeight());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisable(GL_TEXTURE_2D);
}
