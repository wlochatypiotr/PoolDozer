#pragma once
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

class Configuration
{
public:
	Configuration() = delete;
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;

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

	static void Initialize();
	static void InitializeMarices();
	static void InitializeColors();
	static void InitializeGLFW();
	static void InitializeOpenGL();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	static bool m_keys[1024];
};