#pragma once

#include "IManager.h"

class Window;

// Manager that fires callbacks based on GLFW events
class InputManager final : protected IManager
{
public:
	// keyboard
	typedef std::function<void(Window*, int, int, int, int)> KeyCallback;
	// mouse
	typedef std::function<void(Window*, float, float)> MouseMoveCallback;
	typedef std::function<void(Window*, bool)> MouseEnterCallback;
	typedef std::function<void(Window*, int, int, int, float, float)> MouseClickCallback;

private:
	InputManager();
	~InputManager();

	std::list<KeyCallback> m_keyCBs;
	std::list<MouseMoveCallback> m_mouseMoveCBs;
	std::list<MouseEnterCallback> m_mouseEnterCBs;
	std::list<MouseClickCallback> m_mouseClickCBs;

	bool m_LMpressed;

public:
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	static InputManager& instance()
	{
		static InputManager _instance;
		return _instance;
	}

	void update(float _deltaTime) override;
	void initialize() override;
	void keyCallback(Window* win, int key, int scancode, int action, int mods);
	void cursorPosCallback(Window* win, float xpos, float ypos);
	void cursorEnterCallback(Window* win, bool entered);
	void cursorButtonCallback(Window* win, int button, int action, int mods);
	void framebufferSizeCallback(Window* win, int width, int height);

	void addKeyCallback(KeyCallback kcb);
	void addMouseMoveCallback(MouseMoveCallback mmcb);
	void addMouseEnterCallback(MouseEnterCallback mecb);
	void addMouseClickCallback(MouseClickCallback mccb);
	bool isLMousePressed() const;
};

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	InputManager::instance().keyCallback(win, key, scancode, action, mods);
}

static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	InputManager::instance().cursorPosCallback(win, static_cast<float>(xpos), static_cast<float>(ypos));
}

static void CursorEnterCallback(GLFWwindow* window, int entered)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	InputManager::instance().cursorEnterCallback(win, entered == 0 ? false : true);
}

static void CursorButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	InputManager::instance().cursorButtonCallback(win, button, action, mods);
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	InputManager::instance().framebufferSizeCallback(win, width, height);
}

