#include "Configuration.h"
#include "TinyModel.h"
#include "World.h"
#include "PhysXWorld.h"
#include "Entity.h"
#include "Renderer.h"

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	
	Configuration::Initialize();
	//init PhysX
	PhysXWorld physXworld;
	physXworld.StartUp();

	World m_World;
	Renderer renderer;
	renderer.Initialize(Configuration::m_view, Configuration::m_projection, Configuration::m_shaders.Get(1));
	
	//create entity
	CEntity dice("dice");
	//set transform
	Transform dick;
	dick.m_position = Configuration::m_lightPosition; //{ 0.0f, 1.0f, -5.0f};
	dick.m_rotation = { 0.0f, 95.0f, 0.0f };
	dick.m_scale = { 0.5f, 0.5f, 0.5f };
	dice.SetTransform(dick);

	//create component and fill it's values
	CECVisualMesh cubeMesh;
	cubeMesh.SetColor(1.0f, 0.0f, 0.0f);
	cubeMesh.SetProgram(Configuration::m_shaders.Get(0));

	
	//renderer.DrawMesh(&dice);
	//Shaders and program

	TinyModel Lamp("models/Cube.obj"
		, Configuration::m_shaders.Get(Configuration::Shaders::LAMP_SHADER)
		, &Configuration::m_lampModel
		, &Configuration::m_view
		, &Configuration::m_projection
		, &Configuration::m_boardColor
		, &Configuration::m_lightColor
		, &Configuration::m_lightPosition);

	//set components mesh
	cubeMesh.SetMesh(Lamp.m_renderingData);
	dice.SetEntityComponent(&cubeMesh);
	//renderer.DrawMesh(&dice);


	TinyModel Tab("models/PoolTable.obj"
		, Configuration::m_shaders.Get(Configuration::Shaders::TABLE_SHADER)
		, &Configuration::m_boardModel
		, &Configuration::m_view
		, &Configuration::m_projection
		, &Configuration::m_lightColor
		, &Configuration::m_lightColor
		, &Configuration::m_lightPosition);

	
	m_World.AddEntity(&Tab);
	m_World.AddEntity(&Lamp);

	//main loop
	while (!glfwWindowShouldClose(Configuration::m_window))
	{
		// Get frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process events
		Configuration::m_inputManager.ProcessInput();
		//glfwPollEvents();
		//wordl.ProcessInput
		//world.Update()
		

		//drawing functions
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.DrawMesh(&dice);
		//m_World.Draw();
		//Tab.Draw();
		//Lamp.Draw();
		glfwSwapBuffers(Configuration::m_window);
	}
	glfwTerminate();
	physXworld.ShutDown();
	return 0;
}


