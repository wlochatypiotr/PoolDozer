#include "Renderer.h"

CRenderer::CRenderer()
{
}

void CRenderer::Initialize(const Shader& shader, CWindowManager * mgr, int FPSLimit)
{
	//set pointer for windowManager
	WindowManager = mgr;

	//compute view matrix
	ViewMatrix = glm::mat4();
	ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0.0f, 0.0f, -2.6f));
	ViewMatrix = glm::rotate(ViewMatrix, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	
	//compute projection matrix
	ProjectionMatrix = glm::mat4();
	ProjectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)WindowManager->GetWindowWidth() / (GLfloat)WindowManager->GetWindowHeight(), 0.1f, 100.0f);

	//set default shader
	SetProgram(shader);

	//setup some GL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void CRenderer::SetViewMatrix(const mat4& view)
{
	ViewMatrix = view;
}

void CRenderer::SetProjectionMatrix(const mat4 & proj)
{
	ProjectionMatrix = proj;
}

void CRenderer::SetProgram(const Shader & shader)
{
	DefaultShader = shader;
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
		glUseProgram(DefaultShader.Program);
		program = DefaultShader.Program;
	}

	//hardcoede matrix uniform location
	glUniformMatrix4fv(EUniformEnum::MODEL_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(meshComponent->m_model));
	glUniformMatrix4fv(EUniformEnum::VIEW_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(EUniformEnum::PROJECTION_MATRIX_4X4, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

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
		if (bIsTexturingEnabled && c.HasTextures())
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

void CRenderer::Draw(CWorld * World)
{
	ClearBuffer();
	using TIterator = std::unordered_map <scene_id, std::unique_ptr<CScene> >::const_iterator;
		for (TIterator Iterator = World->GetLevels().cbegin(); Iterator != World->GetLevels().cend(); ++Iterator)
		{
			Draw(Iterator->second.get());
		}
	SwapBuffer();
}

void CRenderer::EnableTexturing(bool flag)
{
	bIsTexturingEnabled = flag;
}

//this will be moved to draw scene method
void CRenderer::ClearBuffer()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WindowManager->GetWindowWidth(), WindowManager->GetWindowHeight());
}

void CRenderer::SwapBuffer()
{
	glfwSwapBuffers(WindowManager->GetWindow());
}

