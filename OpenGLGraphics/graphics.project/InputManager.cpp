#include "Core.h"
#include "InputManager.h"

#include "RenderManager.h"

#include "Window.h"

InputManager::InputManager()
	: IManager(TO_STRING(InputManager))
{
}


InputManager::~InputManager()
{
}

void InputManager::update(float _deltaTime)
{
	m_LMpressed = false;
	glfwPollEvents();
	auto win = RenderManager::instance().getWindow();
	if (win == nullptr)
	{
		std::cout << "InputManager could not get active window" << std::endl;
	}
	if (glfwWindowShouldClose(win->getWindow()))
	{
		Engine::instance().requestShutdown();
	}
}

void InputManager::initialize()
{
}

void InputManager::keyCallback(Window * win, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		Engine::instance().requestShutdown();
	}
	for (auto& cb : m_keyCBs)
	{
		cb(win, key, scancode, action, mods);
	}
}

void InputManager::cursorPosCallback(Window * win, float xpos, float ypos)
{
	for (auto& cb : m_mouseMoveCBs)
	{
		cb(win, xpos, ypos);
	}
	glfwSetCursorPos(win->getWindow(), win->getMidX(), win->getMidY());
}

void InputManager::cursorEnterCallback(Window * win, bool entered)
{
	for (auto& cb : m_mouseEnterCBs)
	{
		cb(win, entered);
	}
}

void InputManager::cursorButtonCallback(Window * win, int button, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		if (button == GLFW_MOUSE_BUTTON_1)
		{
			m_LMpressed = true;
		}
	}
	else {
		m_LMpressed = false;
	}
	for (auto& cb : m_mouseClickCBs)
	{
		double x, y;
		glfwGetCursorPos(win->getWindow(), &x, &y);
		cb(win, button, action, mods, static_cast<float>(x), static_cast<float>(y));
	}
}

void InputManager::framebufferSizeCallback(Window * win, int width, int height)
{
	RenderManager::instance().resize(width, height);
}

void InputManager::addKeyCallback(KeyCallback kcb)
{
	m_keyCBs.push_back(kcb);
}

void InputManager::addMouseMoveCallback(MouseMoveCallback mmcb)
{
	m_mouseMoveCBs.push_back(mmcb);
}

void InputManager::addMouseEnterCallback(MouseEnterCallback mecb)
{
	m_mouseEnterCBs.push_back(mecb);
}

void InputManager::addMouseClickCallback(MouseClickCallback mccb)
{
	m_mouseClickCBs.push_back(mccb);
}

bool InputManager::isLMousePressed() const
{
	return m_LMpressed;
}
