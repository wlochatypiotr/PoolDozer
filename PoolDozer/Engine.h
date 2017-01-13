#pragma once
#include "ShaderManager.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "ShaderManager.h"
#include "MeshManager.h"
#include "ComponentManager.h"
#include "World.h"
#include "Renderer.h"
#include "PhysXWorld.h"
#include <memory>

class CEngine
{
public:
	CEngine();
	~CEngine();
	CEngine(const CEngine&) = delete;
	CEngine& operator=(const CEngine&) = delete;
	bool StartUp();
	void ShutDown();
	double GetCurrentTime();
	CWindowManager * GetWindowManager() const { return m_windowManager.get();  }
	CInputManager * GetInputManager() const { return m_inputManager.get(); }
	CShaderManager * GetShaderManager() const { return m_shaderManager.get(); }
	CMeshManager * GetMeshManager() const { return m_meshManager.get(); }
	CRenderer * GetRenderer() const { return m_renderer.get(); }
	CComponentManager * GetComponentManager() const { return m_componentManager.get(); }
	CWorld * GetWorld() const { return m_world.get(); }

private:
	std::unique_ptr<CWindowManager> m_windowManager;
	std::unique_ptr<CInputManager> m_inputManager;
	std::unique_ptr<CShaderManager> m_shaderManager;
	std::unique_ptr<CRenderer> m_renderer;
	std::unique_ptr<CMeshManager> m_meshManager;
	std::unique_ptr<CComponentManager> m_componentManager;
	std::unique_ptr<CPhysXWorld> m_physXWorld;
	std::unique_ptr<CWorld> m_world;

};

