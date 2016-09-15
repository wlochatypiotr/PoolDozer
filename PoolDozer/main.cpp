#include "Drawable.h"
#include "FBXModel.h"
#include "ModelsData.h"
#include "PxPhysicsAPI.h"
#include "Configuration.h"


int main()
{
	
	Configuration::Initialize();

	//Shaders and program

	ShaderLoader BoardShader("VS.frag", "FS.frag");
	ShaderLoader LampShader("VS.frag", "LampShader.frag");

	Drawable Dice(LampShader, models::DiceVertex, 24, models::DiceIndex, 36 );
	FBXModel Table("Models/PoolTable.fbx", BoardShader);

	//main loop
	while (!glfwWindowShouldClose(Configuration::m_window))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Table.Draw(Configuration::m_boardModel, Configuration::m_view, Configuration::m_projection, Configuration::m_boardColor, Configuration::m_lightColor);
	
		glUseProgram(Dice.GetProgram());
		glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(Configuration::m_lampModel));
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(Configuration::m_view));
		glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(Configuration::m_projection));
		Dice.Draw();
		glfwSwapBuffers(Configuration::m_window);
	}
	glfwTerminate();
	return 0;
}


