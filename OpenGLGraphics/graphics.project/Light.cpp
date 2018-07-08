#include "Core.h"
#include "Light.h"

IMPLEMENT_ABSTRACT_CLASS(Light);

#include "ShaderProgram.h"
#include "RenderManager.h"

Light::Light()
{
}

Light::~Light()
{
}

Light::TYPE Light::getLightType() const
{
	return m_lightType;
}

void Light::initialize()
{
	Component::initialize();
	RenderManager::instance().addLight(std::static_pointer_cast<Light>(shared_from_this()));
}

void Light::update(float _deltaTime)
{
	Component::update(_deltaTime);
}

void Light::load(XMLElement * element)
{
	Component::load(element);
	XMLElement* ambient = element->FirstChildElement("ambient");
	if (ambient != nullptr)
	{
		XMLElement* x = ambient->FirstChildElement("x");
		if (x != nullptr)
		{
			x->QueryFloatText(&m_ambient.x);
		}

		XMLElement* y = ambient->FirstChildElement("y");
		if (y != nullptr)
		{
			y->QueryFloatText(&m_ambient.y);
		}

		XMLElement* z = ambient->FirstChildElement("z");
		if (z != nullptr)
		{
			z->QueryFloatText(&m_ambient.z);
		}
	}

	XMLElement* diffuse = element->FirstChildElement("diffuse");
	if (diffuse != nullptr)
	{
		XMLElement* x = diffuse->FirstChildElement("x");
		if (x != nullptr)
		{
			x->QueryFloatText(&m_diffuse.x);
		}

		XMLElement* y = diffuse->FirstChildElement("y");
		if (y != nullptr)
		{
			y->QueryFloatText(&m_diffuse.y);
		}

		XMLElement* z = diffuse->FirstChildElement("z");
		if (z != nullptr)
		{
			z->QueryFloatText(&m_diffuse.z);
		}
	}

	XMLElement* specular = element->FirstChildElement("specular");
	if (specular != nullptr)
	{
		XMLElement* x = specular->FirstChildElement("x");
		if (x != nullptr)
		{
			x->QueryFloatText(&m_specular.x);
		}

		XMLElement* y = specular->FirstChildElement("y");
		if (y != nullptr)
		{
			y->QueryFloatText(&m_specular.y);
		}

		XMLElement* z = specular->FirstChildElement("z");
		if (z != nullptr)
		{
			z->QueryFloatText(&m_specular.z);
		}
	}
}

void Light::bind(int index, ShaderProgram * program)
{
}