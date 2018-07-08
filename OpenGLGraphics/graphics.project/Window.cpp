#include "Core.h"
#include "Window.h"

#include "CameraComponent.h"
#include "GameObjectManager.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "InputManager.h"

IMPLEMENT_DYNAMIC_CLASS(Window);

#ifdef DEBUG

void APIENTRY error_cb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (id == 131185)
	{
		return;
	}

	std::cout << "==========  OPENGL INFO  ==========" << std::endl;
	std::cout << "id: " << id << std::endl;
	std::cout << "severity: ";
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "HIGH" << std::endl;
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "MEDIUM" << std::endl;
		break;
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "LOW" << std::endl;
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "NOTIFICATION" << std::endl;
		break;
	}
	std::cout << "source: ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		std::cout << "API" << std::endl;
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cout << "WINDOW SYSTEM" << std::endl;
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cout << "SHADER COMPILER" << std::endl;
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cout << "THIRD PARTY" << std::endl;
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::cout << "APPLICATION" << std::endl;
		break;
	case GL_DEBUG_SOURCE_OTHER:
		std::cout << "OTHER" << std::endl;
		break;
	}
	std::cout << "type: ";
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "ERROR" << std::endl;
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "DEPRECATED BEHAVIOUR" << std::endl;
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "UNDEFINED BEHAVIOUR" << std::endl;
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "PORTABILITY" << std::endl;
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "PERFORMANCE" << std::endl;
		break;
	case GL_DEBUG_TYPE_MARKER:
		std::cout << "MARKER" << std::endl;
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		std::cout << "PUSH GROUP" << std::endl;
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		std::cout << "POP GROUP" << std::endl;
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "OTHER" << std::endl;
		break;
	}
	std::cout << "message: " << message << std::endl;
}

#endif

Window::Window()
	: m_fullscreen(false),
	m_width(0),
	m_height(0)
{
}


Window::~Window()
{
}

void Window::load(tinyxml2::XMLElement * element)
{
	Object::load(element);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_SAMPLES, 4);

	tinyxml2::XMLElement* width = element->FirstChildElement("width");
	if (width == nullptr)
	{
		std::cout << "Window could not load width" << std::endl;
		return;
	}

	width->QueryIntText(&m_width);
	m_midX = m_width / 2.0f;

	tinyxml2::XMLElement* height = element->FirstChildElement("height");
	if (height == nullptr)
	{
		std::cout << "Window could not load height" << std::endl;
		return;
	}

	height->QueryIntText(&m_height);
	m_midY = m_height / 2.0f;

	tinyxml2::XMLElement* title = element->FirstChildElement("title");
	if (title == nullptr)
	{
		std::cout << "Window could not load title" << std::endl;
		return;
	}

	m_title = title->GetText();

	tinyxml2::XMLElement* fullscreen = element->FirstChildElement("fullscreen");
	if (fullscreen != nullptr)
	{
		element->QueryBoolText(&m_fullscreen);
	}

	//! TODO implement monitor
	m_window = std::unique_ptr<GLFWwindow, glfw_deleter>(glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr));

	if (m_window == nullptr)
	{
		std::cout << "Window creation failed" << std::endl;
		return;
	}

	glfwSetCursorPos(m_window.get(), m_midX, m_midY);
}

void Window::update(float _deltaTime)
{
	Object::update(_deltaTime);
}

void Window::initialize()
{
	Object::initialize();

	glfwSetInputMode(m_window.get(), GLFW_STICKY_KEYS, 1);
	glfwSetInputMode(m_window.get(), GLFW_STICKY_MOUSE_BUTTONS, 1);

	glfwMakeContextCurrent(m_window.get());

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW INIT FAILED!" << std::endl;
		exit(EXIT_FAILURE);
	}

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(error_cb, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
#endif // !NDEBUG

	LOG("VENDOR: " << glGetString(GL_VENDOR));
	LOG("RENDERER: " << glGetString(GL_RENDERER));
	LOG("VERSION: " << glGetString(GL_VERSION));
	LOG("GLSL VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_MULTISAMPLE);

	glfwSetWindowUserPointer(m_window.get(), this);

	glfwSetKeyCallback(m_window.get(), KeyCallback);
	glfwSetCursorPosCallback(m_window.get(), CursorPosCallback);
	glfwSetCursorEnterCallback(m_window.get(), CursorEnterCallback);
	glfwSetMouseButtonCallback(m_window.get(), CursorButtonCallback);
	glfwSetFramebufferSizeCallback(m_window.get(), FramebufferSizeCallback);
}

void Window::resize(int width, int height)
{
	m_width = width;
	m_height = height;
	auto camera = GameObjectManager::instance().getCamera();
	if (camera != nullptr)
	{
		auto cc = std::static_pointer_cast<CameraComponent>(camera->getComponent<CameraComponent>());
		if (cc != nullptr)
		{
			cc->setDimensions(m_width, m_height);
		}
	}
}

const int Window::getWidth() const
{
	return m_width;
}

const int Window::getHeight() const
{
	return m_height;
}

GLFWwindow * Window::getWindow() const
{
	return m_window.get();
}

const float Window::getMidX() const
{
	return m_midX;
}

const float Window::getMidY() const
{
	return m_midY;
}
