#include "Engine.h"
#include <thread>
//#include "Model.h"
//TODO:
//transform CMesh to CModel containing meshes
//load textures and store them in TextureManager
//adapt shaders to use of textures

constexpr int FRAME_LIMIT = 60;

//TODO: add default position, rotation scale etc. values foe entities
int main()
{
	CEngine Engine;
	Engine.StartUp();
	Engine.SetFrameLimit(FRAME_LIMIT);

	//1. Load models using MeshManager
	//2. Create Entity (or use manager(world) in future)
	//3. Create EntityComponent
	//4. Set EntityComponent properties
	//5. Set Entity properties and components

	////////////////////////1///////////////////////////////
	//Takes place in engines MeshManager::Initialze()

	////////////////////////2///////////////////////////////
	//Get world instance
	CWorld* World = Engine.GetWorld();

	//Create new scene and populate it
	CScene* Scene = World->AddScene("level_1");

	Scene->AddNewEntity("entity_lamp");
	Scene->AddNewEntity("entity_table");
	Scene->AddNewEntity("entity_dozer");
	
	/////////////////////////3//////////////////////////////
	World->AddComponent("component_mesh_cube", MESH_COMPONENT);
	World->AddComponent("component_mesh_table", MESH_COMPONENT);
	World->AddComponent("component_mesh_dozer", MESH_COMPONENT);


	//////////////////////////4/////////////////////////////
	CECVisualMesh* LampComponent = static_cast<CECVisualMesh*>(World->GetComponent("component_mesh_cube"));
	CECVisualMesh* TableComponent = static_cast<CECVisualMesh*>(World->GetComponent("component_mesh_table"));
	CECVisualMesh* DozerComponent = static_cast<CECVisualMesh*>(World->GetComponent("component_mesh_dozer"));

	LampComponent->SetColor(1.0f, 0.5f, 0.5f);
	LampComponent->SetProgram(Engine.GetShaderManager()->Get(0));
	LampComponent->SetMesh(Engine.GetMeshManager()->Get("mesh_cube"));

	TableComponent->SetColor(0.5f, 0.5f, 0.5f);
	TableComponent->SetProgram(Engine.GetShaderManager()->Get(1));
	TableComponent->SetMesh(Engine.GetMeshManager()->Get("mesh_table"));

	DozerComponent->SetColor(1.0f, 1.0f, 1.0f);
	DozerComponent->SetProgram(Engine.GetShaderManager()->Get(1));
	DozerComponent->SetMesh(Engine.GetMeshManager()->Get("mesh_dozer"));

	////////////////////////////5//////////////////////////
	CEntity* Lamp = Scene->GetEntity("entity_lamp");
	Lamp->SetPosition(glm::vec3(0.0f, 0.5f, -6.0f));
	Lamp->SetRotation(0.0f, 0.0f, 0.0f);
	Lamp->SetScale(0.5f);
	Lamp->SetEntityComponent(LampComponent);
	Lamp->SetVisible(false);

	CEntity* Table = Scene->GetEntity("entity_table");
	Table->SetPosition(glm::vec3(0.0f, -1.0f, -1.40f));
	Table->SetRotation(0.0f, 0.0f, 0.0f);
	Table->SetScale(1.0f);
	Table->SetEntityComponent(TableComponent);

	CEntity* Dozer = Scene->GetEntity("entity_dozer");
	Dozer->SetPosition(glm::vec3(0.0f, -0.8f, 2.20f));
	//todo: rotation seems not to work
	Dozer->SetRotation(0.0f, 0.0f, 0.0f);
	Dozer->SetScale(0.07f);
	Dozer->SetEntityComponent(DozerComponent);

	// c-constructor ?
	//set owning entity ?
	//scene->AddNewEntity("dozer_copy", dozer);
	//dozer->SetVisible(false);
	//CEntity nanosuitCopy(*dozer);
	//nanosuitCopy.SetID("nanosuit");
	//nanosuitCopy.SetPosition(glm::vec3(1.0f, -0.8f, 2.20f));

	//main loop
	//engine.GetRenderer()->EnableTexturing(false);
	GLfloat Accumulate = 0;
	double Fun = 0;
	while (!glfwWindowShouldClose(Engine.GetWindowManager()->GetWindow()))
	{
		//update times
		Engine.TickClock();
		double deltaTime = Engine.GetDeltaTime();

		// process events
		Engine.GetInputManager()->ProcessInput();
		World->Update();

		//submit to renderer
		if (Engine.ShouldDrawNextFrame())
		{
			Engine.GetRenderer()->Draw(World);
			Accumulate = 0;
		}
	}
	Engine.ShutDown();

	return 0;
}


