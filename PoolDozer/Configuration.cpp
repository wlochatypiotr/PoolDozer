#include "Configuration.h"

glm::mat4 Configuration::m_view = glm::mat4();
glm::mat4 Configuration::m_projection = glm::mat4();
glm::mat4 Configuration::m_boardModel = glm::mat4();
glm::mat4 Configuration::m_lampModel = glm::mat4();

glm::vec3 Configuration::m_boardColor;
glm::vec3 Configuration::m_lightColor;

GLuint Configuration::m_screenHeight;
GLuint Configuration::m_screenWidth;

bool Configuration::m_keys[1024];

GLFWwindow* Configuration::m_window = nullptr;

void Configuration::Initialize()
{
	InitializeGLFW();
	InitializeMarices();
	InitializeColors();
	InitializeOpenGL();
}

void Configuration::InitializeMarices()
{
	m_view = glm::translate(m_view, glm::vec3(0.0f, 0.0f, -2.5f));
	m_projection = glm::perspective(glm::radians(45.0f), (GLfloat)m_screenWidth / (GLfloat)m_screenHeight, 0.1f, 100.0f);

	m_boardModel = glm::rotate(m_boardModel, glm::radians(135.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_boardModel = glm::translate(m_boardModel, glm::vec3(0.0f, 0.0f, -1.20f));

	m_lampModel = glm::scale(m_lampModel, glm::vec3(0.5f, 0.5f, 0.5f));
	m_lampModel = glm::translate(m_lampModel, glm::vec3(8.0f, 4.0f, -10.0f));
	m_lampModel = glm::rotate(m_lampModel, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Configuration::InitializeColors()
{
	m_boardColor = glm::vec3 (0.663f, 0.663f, 0.663f);
	m_lightColor = glm::vec3 (1.0f, 1.0f, 1.0f);
}

void Configuration::InitializeGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glewExperimental = GL_TRUE;

	//windows setup
	m_screenWidth = 1366;
	m_screenHeight = 768;
	m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, "PoolDozer", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		//return -1;
		//throw some exception ?
	}
	//register callback functions
	glfwSetKeyCallback(m_window, KeyCallback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Configuration::InitializeOpenGL()
{
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Configuration::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			m_keys[key] = true;
		else if (action == GLFW_RELEASE)
			m_keys[key] = false;
	}
}
