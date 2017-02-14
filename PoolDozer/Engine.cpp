#include "Engine.h"

CEngine::CEngine()
{
}

CEngine::~CEngine()
{

}

bool CEngine::StartUp()
{
	//1
	//init windowManager
	GLuint ret = glGetError();
	m_windowManager = std::make_unique<CWindowManager>();
	m_windowManager->Initialize(4, 3, 1366, 766);
	ret = glGetError();
	//2
	//init input manager - move to initialize
	m_inputManager = std::make_unique<CInputManager>();
	m_inputManager->Initialize(m_windowManager->GetWindow());
	ret = glGetError();
	//3
	//init shader mgr and load shaders in Initialize method
	m_shaderManager = std::make_unique<CShaderManager>();
	m_shaderManager->Initialize();
	//4
	//init MeshManager and load some meshes
	m_meshManager = std::make_unique<CModelManager>();
	m_meshManager->Initialize();

	//5
	m_componentManager = std::make_unique<CComponentManager>();
	//loadcomponents
	//m_componentManager->Initialize();

	//6
	//init Renderer with s default shader
	m_renderer = std::make_unique<CRenderer>();
	m_renderer->Initialize(m_shaderManager->Get(m_shaderManager->TABLE_SHADER), this->GetWindowManager());
	//7

	//Initialzie PhysX engine
	m_physXWorld = std::make_unique<CPhysXWorld>();
	m_physXWorld->StartUp();	
	//8 
	//Init world
	m_world = std::make_unique<CWorld>();
	m_world->Initialize(GetComponentManager());

	return false;
}

void CEngine::ShutDown()
{
	//8
	//shutdown world

	//7
	m_physXWorld->ShutDown();

	//6
	//m_renderer->ShutDown();

	//5
	//m_componentManager->ShutDown();

	//4
	m_meshManager->ShutDown();

	//3
	//m_shaderManager->ShutDown();

	//2
	//m_inputManager->ShutDown();

	//1
	m_windowManager->ShutDown();
}

double CEngine::GetCurrentTime()
{
	return glfwGetTime();
}
