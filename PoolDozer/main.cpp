#include "log.h"
#include "Drawable.h"
#include "ModelsData.h"
#include "Configuration.h"
#include "TinyModel.h"
#include "World.h"
//#include "PhysXWorld.h"
#include "PxPhysicsAPI.h"

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	
	Configuration::Initialize();

	//init PhysX
	//static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	//static physx::PxDefaultAllocator gDefaultAllocatorCallback;

	//physx::PxFoundation* mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
	//	gDefaultErrorCallback);
	//if (!mFoundation)
	//	gDefaultErrorCallback.reportError(physx::PxErrorCode::eABORT,"PxCreateFoundation failed!",__FILE__, __LINE__);
	//
	//coœ nie dzia³a ze œwiatem !?
	World m_World;

	//Shaders and program


	//ShaderLoader BoardShader("VS.frag", "FS.frag");
	//ShaderLoader LampShader("VS.frag", "LampShader.frag");

	TinyModel Lamp("Models/Cube.obj"
		, Configuration::m_shaders.Get(Configuration::Shaders::LAMP_SHADER)
		, &Configuration::m_lampModel
		, &Configuration::m_view
		, &Configuration::m_projection
		, &Configuration::m_boardColor
		, &Configuration::m_lightColor
		, &Configuration::m_lightPosition);

	TinyModel Tab("Models/PoolTable.obj"
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
		glfwPollEvents();
		//wordl.ProcessInput
		//world.Update()
		

		//drawing functions
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_World.Draw();
		//Tab.Draw();
		//Lamp.Draw();
		glfwSwapBuffers(Configuration::m_window);
	}
	glfwTerminate();
	return 0;
}


