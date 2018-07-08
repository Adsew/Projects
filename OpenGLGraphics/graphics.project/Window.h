#pragma once

#include "Object.h"

// delete for the GLFW unique_ptr
struct glfw_deleter
{
	void operator()(GLFWwindow* ptr)
	{
		glfwDestroyWindow(ptr);
	}
};

class Window : public Object
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Window, Object);
private:
	int m_width;
	int m_height;
	float m_midX;
	float m_midY;
	std::string m_title;
	bool m_fullscreen;
	std::unique_ptr<GLFWwindow, glfw_deleter> m_window;

public:
	Window();
	~Window();
	void load(tinyxml2::XMLElement* element) override;
	void update(float _deltaTime) override;
	void initialize() override;
	// resize the window
	void resize(int width, int height);
	const int getWidth() const;
	const int getHeight() const;
	// return the GLFW window from the class
	GLFWwindow* getWindow() const;
	const float getMidX() const;
	const float getMidY() const;
};


