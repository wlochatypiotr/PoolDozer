#pragma once
#include "WindowManager.h"
#include "InputManager.h"
#include "ShaderManager.h"
#include <memory>

class CEngine
{
public:
	CEngine();
	CEngine(const CEngine&) = delete;
	CEngine& operator=(const CEngine&) = delete;
	bool StartUp();
	CWindowManager * GetWindowManager() const { return m_windowManager.get();  }
	CInputManager * GetInputManager() const { return m_inputManager.get(); }
	CShaderManager * GetShaderManager() const { return m_shaderManager.get(); }
private:
	std::unique_ptr<CWindowManager> m_windowManager;
	std::unique_ptr<CInputManager> m_inputManager;
	std::unique_ptr<CShaderManager> m_shaderManager;
};


//InitializeColors(); //config 
//InitializeMarices();	//config
//InitializeOpenGL(); //renderer
