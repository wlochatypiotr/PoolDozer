#include "Configuration.h"
#include "World.h"
#include "PhysXWorld.h"
#include "Renderer.h"
#include "MeshManager.h"
#include "Engine.h"


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	CEngine engine;
	engine.StartUp();

	Configuration::Initialize();
	//init PhysX
	PhysXWorld physXworld;
	physXworld.StartUp();

	Renderer renderer;
	renderer.Initialize(engine.GetShaderManager()->Get(1), engine.GetWindowManager());
	
	//1. Load models using MeshManager
	//2. Create Entity (or use manager(world) in future)
	//3. Create EntityComponent
	//4. Set EntityComponent proporties
	//5. Set Enitity proporties and components

	////////////////////////1///////////////////////////////
	MeshManager meshManager;
	meshManager.Load("MCube", "models/Cube.obj");
	meshManager.Load("MTable", "models/PoolTable.obj");

	////////////////////////2///////////////////////////////
	CWorld world(&renderer);
	world.AddNewEntity("Elamp");
	world.AddNewEntity("Etable");

	/////////////////////////3//////////////////////////////
	world.AddComponent("CcubeMesh", CWorld::MESH_COMPONENT);
	world.AddComponent("CtableMesh", CWorld::MESH_COMPONENT);

	//////////////////////////4/////////////////////////////
	auto lampcomp = static_cast<CECVisualMesh*>(world.GetComponent("CcubeMesh"));
	auto tablecomp = static_cast<CECVisualMesh*>(world.GetComponent("CtableMesh"));

	lampcomp->SetColor(1.0f, 0.5f, 0.5f);
	lampcomp->SetProgram(engine.GetShaderManager()->Get(0));
	lampcomp->SetMesh(meshManager.Get("MCube"));

	tablecomp->SetColor(0.5f, 0.5f, 0.5f);
	tablecomp->SetProgram(engine.GetShaderManager()->Get(1));
	tablecomp->SetMesh(meshManager.Get("MTable"));

	////////////////////////////5//////////////////////////
	CEntity* lamp = world.GetEntity("Elamp");
	lamp->SetPosition(Configuration::m_lightPosition);
	lamp->SetRotation(0.0f, 0.0f, 0.0f);
	lamp->SetScale(0.5f);
	lamp->SetEntityComponent(lampcomp);

	CEntity* table = world.GetEntity("Etable");
	table->SetPosition(glm::vec3(0.0f, -1.0f, -1.40f));
	table->SetRotation(0.0f, 0.0f, 0.0f);
	table->SetScale(1.0f);
	table->SetEntityComponent(tablecomp);

	//main loop
	while (!glfwWindowShouldClose(engine.GetWindowManager()->GetWindow()))
	{
		// Get frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process events
		engine.GetInputManager()->ProcessInput();
		world.Update();
		world.Draw();

		glfwSwapBuffers(engine.GetWindowManager()->GetWindow());
	}
	glfwTerminate();
	physXworld.ShutDown();

	return 0;
}


