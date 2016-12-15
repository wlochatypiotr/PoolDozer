#pragma once
#include "ShaderLoader.h" // add GL/glew
#include "InputManager.h"	//add GLFW order here matters
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <thread>
#include "ShaderManager.h"
#include "InputManager.h"


class Configuration
{
public:
	Configuration() = delete;
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;

	//no of threads
	static unsigned int m_threadsNumber;

	//managers
	static CShaderManager m_shaders;
	static CInputManager m_inputManager;

	enum Shaders
	{
		LAMP_SHADER,
		TABLE_SHADER
	};
	//windows attributes
	static GLuint m_screenWidth;
	static GLuint m_screenHeight;
	static GLFWwindow* m_window;

	//matrices
	static glm::mat4 m_view;
	static glm::mat4 m_projection;

	static glm::mat4 m_boardModel;
	static glm::mat4 m_lampModel;

	static glm::vec3 m_boardColor;
	static glm::vec3 m_lightColor;

	static glm::vec3 m_lightPosition;

	//vertex atributes layout positions

	enum m_vertexAttributes {
		VERTEX_POSITION = 0,
		VERTEX_NORMAL = 1,
		MODEL_MATRIX = 3,
		VIEW_MATRIX = 4,
		PROJECTION_MATRIX = 5,
		OBJECT_COLOR = 6,
		LIGHT_COLOR = 7,
		LIGHT_SOURCE_POS = 8
	};

	static void Initialize();
	static void InitializeShaders();
	static void InitializeMarices();
	static void InitializeColors();
	static void InitializeGLFW();
	static void InitializeInputManager();
	static void InitializeOpenGL();

	//static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	//static bool m_keys[1024];
};