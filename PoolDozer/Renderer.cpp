#include "Renderer.h"

CRenderer::CRenderer()
{
}

void CRenderer::Initialize(const Shader& shader, CWindowManager * mgr)
{
	//set pointer for windowManager
	m_windowManager = mgr;

	//compute view matrix
	m_view = glm::mat4();
	m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, -2.6f));
	m_view = glm::rotate(m_view, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	
	//compute projection matrix
	m_projection = glm::mat4();
	m_projection = glm::perspective(glm::radians(45.0f), (GLfloat)m_windowManager->GetWindowWidth() / (GLfloat)m_windowManager->GetWindowHeight(), 0.1f, 100.0f);

	//set default shader
	SetProgram(shader);

	//setup some GL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void CRenderer::SetViewMatrix(const mat4& view)
{
	m_view = view;
}

void CRenderer::SetProjectionMatrix(const mat4 & proj)
{
	m_projection = proj;
}

void CRenderer::SetProgram(const Shader & shader)
{
	m_program = shader;
}



void CRenderer::DrawComponent(CECVisualMesh * meshComponent)
{
	GLuint program;
	if (meshComponent->m_program != nullptr)
	{
		glUseProgram(meshComponent->m_program->Program);
		program = meshComponent->m_program->Program;
	}
	else
	{
		glUseProgram(m_program.Program);
		program = m_program.Program;
	}

	//hardcoede matrix uniform location
	glUniformMatrix4fv(EUniformEnum::MODEL_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(meshComponent->m_model));
	glUniformMatrix4fv(EUniformEnum::VIEW_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(m_view));
	glUniformMatrix4fv(EUniformEnum::PROJECTION_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(m_projection));

	glUniform3f(EUniformEnum::OBJECT_COLOR_VEC3, meshComponent->m_color.x, meshComponent->m_color.y, meshComponent->m_color.z);

	//this uniform have use for light source
	//hardcoded light value
	//glUniform3f(EUniformEnum::LIGHT_COLOR_VEC3, 1.0f, 1.0f, 1.0f);
	
	//this uniform have use for object lightning
	//hardcoded light position
	//glUniform3f(EUniformEnum::LIGHT_POS_VEC3, 2 *sin(glfwGetTime()), 1.0, 1.0f);

	//hardcoded camera position, need to be updated
	glUniform3f(CAMERA_POS_VEC3, 0.0f, 0.0f, 0.0f);

	//setup material
	//GLint matAmbientLoc = glGetUniformLocation(program, "u_material.ambient");
	//GLint matDiffuseLoc = glGetUniformLocation(program, "u_material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(program, "u_material.specular");
	GLint matShineLoc = glGetUniformLocation(program, "u_material.shininess");

	//glUniform3f(matAmbientLoc, 0.0f, 0.1f, 0.06f);
	//glUniform3f(matDiffuseLoc, 0.0f, 0.50980392f, 0.50980392f);
	glUniform3f(matSpecularLoc, 0.50196078f, 0.50196078f, 0.50196078f);
	glUniform1f(matShineLoc, 16.0f);

	//setup light
	GLint lightDirectionLoc = glGetUniformLocation(program, "u_light.direction");
	GLint lightAmbientLoc = glGetUniformLocation(program, "u_light.ambient");
	GLint lightDiffuseLoc = glGetUniformLocation(program, "u_light.diffuse");
	GLint lightSpecularLoc = glGetUniformLocation(program, "u_light.specular");
	GLint ligtPositionLoc = glGetUniformLocation(program, "u_light.position");

	glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
	glUniform3f(lightDiffuseLoc, 1.0f, 1.0f, 1.0f); 
	glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(ligtPositionLoc, 3 * sin(glfwGetTime()), 3.0f, 2.0f);
	//directional light
	glUniform3f(lightDirectionLoc, 3 * sin(glfwGetTime()), 3.0f, -2.0f);


	for (const CMesh& c : meshComponent->GetModel()->GetMeshes())
	{
		//if mesh has textures use them as material
		if (m_isTexturingEnabled && c.HasTextures())
		{
			GLuint diffuseNr = 1;
			GLuint specularNr = 1;
			for (GLuint i = 0; i < c.m_textures.size(); ++i)
			{
				std::stringstream ss;
				std::string number;
				std::string name = c.m_textures.at(i).type;
				if (name == "texture_diffuse")
					ss << diffuseNr++;
				else if (name == "texture_specular")
					ss << specularNr++;
				number = ss.str();

				glUniform1i(glGetUniformLocation(program, ("u_material." + name + number).c_str()), i);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, c.m_textures.at(i).id);
			}

		}
		glBindVertexArray(c.m_mesh.VAO);
		glDrawElements(GL_TRIANGLES, c.m_mesh.numIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	glUseProgram(0);
}


void CRenderer::Draw(CEntity * entity)
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


void CRenderer::Draw(CScene * scene)
{
	if (scene->IsActive()) {
		using iterator_t = std::unordered_map <entity_id_t, std::unique_ptr<CEntity> >::const_iterator;
		for (iterator_t iter = scene->GetEntities().cbegin(); iter != scene->GetEntities().cend(); ++iter)
		{
			if(iter->second.get()->IsVisible())
				Draw(iter->second.get());
		}
	}

}

void CRenderer::Draw(CWorld * world)
{
	ClearBuffer();
	using iterator_t = std::unordered_map <scene_id, std::unique_ptr<CScene> >::const_iterator;
		for (iterator_t iter = world->GetLevels().cbegin(); iter != world->GetLevels().cend(); ++iter)
		{
			Draw(iter->second.get());
		}
	SwapBuffer();
}

void CRenderer::EnableTexturing(bool flag)
{
	m_isTexturingEnabled = flag;
}

//this will be moved to draw scene method
void CRenderer::ClearBuffer()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_windowManager->GetWindowWidth(), m_windowManager->GetWindowHeight());
}

void CRenderer::SwapBuffer()
{
	glfwSwapBuffers(m_windowManager->GetWindow());
}

