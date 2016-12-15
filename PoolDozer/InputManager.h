#pragma once
#include "GLFW/glfw3.h"


class CInputManager
{
public:
	CInputManager();
	CInputManager(GLFWwindow* window);
	CInputManager(const CInputManager &) = delete;
	CInputManager& operator=(const CInputManager &) = delete;
	static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
	void SetupKeyCallback();
	static void ProcessInput();
	static bool m_keys[1024];
	static GLFWwindow * m_window;
};