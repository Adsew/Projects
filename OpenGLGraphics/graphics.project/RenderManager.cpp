#include "Core.h"
#include "RenderManager.h"

#include "Light.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

#include "Window.h"
#include "IRenderable.h"

#include "ShaderProgram.h"

RenderManager::RenderManager()
	: IManager(TO_STRING(RenderManager)),
	m_window(std::make_unique<Window>())
{
}


RenderManager::~RenderManager()
{
}

void RenderManager::update(float _deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.7f, 0.87f, 1.0f, 1.0f);
	m_window->update(_deltaTime);

	glEnable(GL_DEPTH_TEST);
	for (auto it = m_renderables.begin(); it != m_renderables.end(); /*blank*/)
	{
		if (!(*it).expired())
		{
			if (auto r = (*it).lock())
			{
				r->render();
			}
			++it;
		}
		else {
			m_renderables.erase(it++);
		}
	}
	glDisable(GL_DEPTH_TEST);
	for (auto it = m_ui_renderables.begin(); it != m_ui_renderables.end(); /*blank*/)
	{
		if (!(*it).expired())
		{
			if (auto r = (*it).lock())
			{
				r->render();
			}
			++it;
		}
		else {
			m_ui_renderables.erase(it++);
		}
	}
	glfwSwapBuffers(m_window->getWindow());
}

void RenderManager::initialize()
{
	std::string path;
	path.append("Config/").append(m_name).append(".xml");

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(path.c_str());

	if (error != tinyxml2::XML_NO_ERROR)
	{
		doc.PrintError();
	}

	tinyxml2::XMLElement* root = doc.FirstChildElement(m_name.c_str());
	if (root == nullptr)
	{
		std::cout << "RenderManager could not find root element" << std::endl;
		return;
	}

	tinyxml2::XMLElement* window = root->FirstChildElement(TO_STRING(Window));
	if (window == nullptr)
	{
		std::cout << "RenderManager could not find window element" << std::endl;
		return;
	}

	// load and initialize glfw/glew
	m_window->load(window);
	m_window->initialize();

	// Enable alpha values
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCullFace(GL_BACK);

	glEnable(GL_TEXTURE_2D);
}

void RenderManager::resize(int width, int height)
{
	m_window->resize(width, height);
}

void RenderManager::attachRenderable(std::shared_ptr<IRenderable> obj)
{
	if (obj == nullptr) { return; }
	m_renderables.emplace_back(obj);
}

void RenderManager::attachUIRenderable(std::shared_ptr<IRenderable> obj, bool front)
{
	if (obj == nullptr) { return; }
	if (front)
	{
		m_ui_renderables.emplace_front(obj);
	}
	else
	{
		m_ui_renderables.emplace_back(obj);
	}
}

void RenderManager::detachRenderable(std::shared_ptr<IRenderable> obj)
{
	if (obj == nullptr) { return; }
	auto code = obj->getUUIDHashCode();
	m_renderables.remove_if([code](auto& renderable) {
		if (auto r = renderable.lock())
		{
			return r->getUUIDHashCode() == code;
		}
		return false;
	});
}

void RenderManager::detachUIRenderable(std::shared_ptr<IRenderable> obj)
{
	if (obj == nullptr) { return; }
	auto code = obj->getUUIDHashCode();
	m_ui_renderables.remove_if([code](auto& renderable) {
		if (auto r = renderable.lock())
		{
			return r->getUUIDHashCode() == code;
		}
		return false;
	});
}

const int RenderManager::getWidth() const
{
	return m_window->getWidth();
}

const int RenderManager::getHeight() const
{
	return m_window->getHeight();
}

const Window * RenderManager::getWindow() const
{
	return m_window.get();
}

void RenderManager::addLight(std::shared_ptr<Light> light)
{
	if (light != nullptr)
	{
		if (light->getLightType() == Light::TYPE::kDirectional)
		{
			m_directionalLight = std::static_pointer_cast<DirectionalLight>(light);
		}
        else if (light->getLightType() == Light::TYPE::kSpot)
        {
            m_spotLight = std::static_pointer_cast<SpotLight>(light);
        }
		else {
			m_lights.push_back(light);
		}
	} ELSE_LOG("Cannot add nullptr light to RenderManager lights")
}

void RenderManager::bindLights(ShaderProgram * program)
{
	if (program == nullptr)
	{
		LOG("RenderManager found null while uploading lights");
		return;
	}

	// bind the directional light
	if (auto dLight = m_directionalLight.lock())
	{
		dLight->bind(0, program);

	}

    // bind the spot light
    if (auto sLight = m_spotLight.lock())
    {
        sLight->bind(0, program);

    }

	// bind the point lights
	for (unsigned int i = 0; i < m_lights.size(); ++i)
	{
		if (auto light = m_lights.at(i).lock())
		{
			light->bind(i, program);
		}
	}
}
