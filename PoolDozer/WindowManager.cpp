#include "WindowManager.h"



CWindowManager::CWindowManager()
{
}


CWindowManager::~CWindowManager()
{
}

void CWindowManager::Initialize(int majorVersion, int minorVersion, int screenWidth, int screenHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glewExperimental = GL_TRUE;

	//windows setup
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, "PoolDozer", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
	}
	else
		std::cout << "Successfully created GLFW window " << m_screenWidth << " x " << m_screenHeight << std::endl;

	//move to renderer
	//glViewport(0, 0, screenWidth, screenHeight);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glDisable(GL_TEXTURE_2D);
}

