#pragma once
#include "GLFW/glfw3.h"


class InputManager
{
public:
	InputManager();
	InputManager(GLFWwindow* window);
	InputManager(const InputManager &) = delete;
	InputManager& operator=(const InputManager &) = delete;
	static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
	void SetupKeyCallback();
	static void ProcessInput();
	static bool m_keys[1024];
	static GLFWwindow * m_window;
};