#include "Engine.h"
//#include "Model.h"
//TODO:
//transform CMesh to CModel containing meshes
//load textures and store them in TextureManager
//adapt shaders to use of textures

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//TODO: add default position, rotation scale etc. values foe entities
int main()
{
	CEngine engine;
	engine.StartUp();

	//1. Load models using MeshManager
	//2. Create Entity (or use manager(world) in future)
	//3. Create EntityComponent
	//4. Set EntityComponent proporties
	//5. Set Enitity proporties and components

	////////////////////////1///////////////////////////////
	//Takes place in engines MeshManager::Initialze()

	////////////////////////2///////////////////////////////
	//Get world instance
	CWorld* world = engine.GetWorld();

	//Create new scene and populate it
	CScene* scene = world->AddScene("level_1");

	scene->AddNewEntity("entity_lamp");
	scene->AddNewEntity("entity_table");
	scene->AddNewEntity("entity_dozer");
	
	/////////////////////////3//////////////////////////////
	world->AddComponent("component_mesh_cube", MESH_COMPONENT);
	world->AddComponent("component_mesh_table", MESH_COMPONENT);
	world->AddComponent("component_mesh_dozer", MESH_COMPONENT);


	//////////////////////////4/////////////////////////////
	CECVisualMesh* lampcomp = static_cast<CECVisualMesh*>(world->GetComponent("component_mesh_cube"));
	CECVisualMesh* tablecomp = static_cast<CECVisualMesh*>(world->GetComponent("component_mesh_table"));
	CECVisualMesh* dozercomp = static_cast<CECVisualMesh*>(world->GetComponent("component_mesh_dozer"));

	lampcomp->SetColor(1.0f, 0.5f, 0.5f);
	lampcomp->SetProgram(engine.GetShaderManager()->Get(0));
	lampcomp->SetMesh(engine.GetMeshManager()->Get("mesh_cube"));

	tablecomp->SetColor(0.5f, 0.5f, 0.5f);
	tablecomp->SetProgram(engine.GetShaderManager()->Get(1));
	tablecomp->SetMesh(engine.GetMeshManager()->Get("mesh_table"));

	dozercomp->SetColor(1.0f, 1.0f, 1.0f);
	dozercomp->SetProgram(engine.GetShaderManager()->Get(1));
	dozercomp->SetMesh(engine.GetMeshManager()->Get("mesh_dozer"));

	////////////////////////////5//////////////////////////
	CEntity* lamp = scene->GetEntity("entity_lamp");
	lamp->SetPosition(glm::vec3(0.0f, 0.5f, -6.0f));
	lamp->SetRotation(0.0f, 0.0f, 0.0f);
	lamp->SetScale(0.5f);
	lamp->SetEntityComponent(lampcomp);

	CEntity* table = scene->GetEntity("entity_table");
	table->SetPosition(glm::vec3(0.0f, -1.0f, -1.40f));
	table->SetRotation(0.0f, 0.0f, 0.0f);
	table->SetScale(1.0f);
	table->SetEntityComponent(tablecomp);

	CEntity* dozer = scene->GetEntity("entity_dozer");
	dozer->SetPosition(glm::vec3(0.0f, -0.8f, -1.40f));
	dozer->SetRotation(0.0f, 0.0f, 0.0f);
	dozer->SetScale(0.07f);
	dozer->SetEntityComponent(dozercomp);

	//main loop
	//engine.GetRenderer()->EnableTexturing(false);
	while (!glfwWindowShouldClose(engine.GetWindowManager()->GetWindow()))
	{
		// Get frame time
		GLfloat currentFrame = engine.GetCurrentTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process events
		engine.GetInputManager()->ProcessInput();
		world->Update();

		//submit to renderer
		engine.GetRenderer()->Draw(world);
	}
	engine.ShutDown();

	return 0;
}


