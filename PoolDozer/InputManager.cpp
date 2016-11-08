#include "InputManager.h"


bool InputManager::m_keys[1024];
GLFWwindow * InputManager::m_window = nullptr;

InputManager::InputManager()
{
}

InputManager::InputManager(GLFWwindow * window)
{
	m_window = window;
	glfwSetKeyCallback(m_window, InputManager::KeyCallback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void InputManager::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			m_keys[key] = true;
		else if (action == GLFW_RELEASE)
			m_keys[key] = false;
	}
}

void InputManager::ProcessInput()
{
	glfwPollEvents();
}

