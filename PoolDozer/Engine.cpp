#include "Engine.h"

CEngine::CEngine()
{
}

bool CEngine::StartUp()
{
	//init windowManager
	m_windowManager = std::make_unique<CWindowManager>();
	m_windowManager->Initialize(3, 3, 1366, 766);
	
	//init input manager - move to initialize
	m_inputManager = std::make_unique<CInputManager>(m_windowManager->GetWindow());

	//init shader mgr
	m_shaderManager = std::make_unique<CShaderManager>();
	//load shaders -- move to initialize
	m_shaderManager->Load(m_shaderManager->LAMP_SHADER, Shader("shaders/VS.frag", "shaders/LampShader.frag"));
	m_shaderManager->Load(m_shaderManager->TABLE_SHADER, Shader("shaders/VS.frag", "shaders/FS.frag"));

	return false;
}
