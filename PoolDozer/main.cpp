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
	
	/////////////////////LOAD MESH//////////////////////////
	TinyModel Lamp("models/Cube.obj"
		, Configuration::m_shaders.Get(Configuration::Shaders::LAMP_SHADER)
		, &Configuration::m_lampModel
		, &Configuration::m_view
		, &Configuration::m_projection
		, &Configuration::m_boardColor
		, &Configuration::m_lightColor
		, &Configuration::m_lightPosition);

	TinyModel Tab("models/PoolTable.obj"
		, Configuration::m_shaders.Get(Configuration::Shaders::TABLE_SHADER)
		, &Configuration::m_boardModel
		, &Configuration::m_view
		, &Configuration::m_projection
		, &Configuration::m_lightColor
		, &Configuration::m_lightColor
		, &Configuration::m_lightPosition);


	//create entity
	CEntity * lamp = new CEntity("lamp");
	//set transform
	lamp->SetPosition(Configuration::m_lightPosition);
	lamp->SetRotation(0.0f, 0.0f, 0.0f);
	lamp->SetScale(0.5f);

	//this could be a shared ptr or unique ptr
	CECVisualMesh * cube = new CECVisualMesh;
	cube->SetColor(0.0f, 0.5f, 0.5f);
	cube->SetProgram(Configuration::m_shaders.Get(0));

	//Tiny model will just load data
	cube->SetMesh(Lamp.m_renderingData);

	lamp->SetEntityComponent(cube);

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

		//now Entity has to have method update which will call Update on all entities
		//dice.Update();
		lamp->Update();
		renderer.Draw(lamp);

		//m_World.Draw();
		//Tab.Draw();
		//Lamp.Draw();
		glfwSwapBuffers(Configuration::m_window);
	}
	glfwTerminate();
	physXworld.ShutDown();

	//free memory
	lamp->ClearComponents();
	delete lamp;
	return 0;
}


