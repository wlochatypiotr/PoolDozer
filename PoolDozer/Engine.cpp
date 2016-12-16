#include "Engine.h"

CEngine::CEngine()
{
}

bool CEngine::StartUp()
{
	//1
	//init windowManager
	m_windowManager = std::make_unique<CWindowManager>();
	m_windowManager->Initialize(3, 3, 1366, 766);
	//2
	//init input manager - move to initialize
	m_inputManager = std::make_unique<CInputManager>();
	m_inputManager->Initialize(m_windowManager->GetWindow());
	//3
	//init shader mgr and load shaders in Initialize method
	m_shaderManager = std::make_unique<CShaderManager>();
	m_shaderManager->Initialize();
	//4
	//init MeshManager and load some meshes
	m_meshManager = std::make_unique<CMeshManager>();
	m_meshManager->Initialize();
	//5
	//init Renderer with s default shader
	m_renderer = std::make_unique<CRenderer>();
	m_renderer->Initialize(m_shaderManager->Get(m_shaderManager->TABLE_SHADER), this->GetWindowManager());
	//6
	//Initialzie PhysX engine
	m_physXWorld = std::make_unique<CPhysXWorld>();
	m_physXWorld->StartUp();
	

	return false;
}

void CEngine::ShutDown()
{
	//6
	m_physXWorld->ShutDown();

	//5
	//m_renderer->ShutDown();

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
