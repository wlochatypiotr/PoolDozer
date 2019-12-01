#pragma once
#include "Gl/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

class CWindowManager
{
public:
	CWindowManager();
	CWindowManager(const CWindowManager&) = delete;
	CWindowManager& operator=(const CWindowManager&) = delete;
	~CWindowManager();
	void Initialize(int majorVersion, int minorVersion, int screenWidth, int screenHeight);
	void ShutDown();
	GLFWwindow * GetWindow() { return m_window; }
	int GetWindowWidth() { return m_screenWidth; }
	int GetWindowHeight() { return m_screenHeight; }

private:
	GLFWwindow * m_window;
	int m_screenWidth;
	int m_screenHeight;
};

