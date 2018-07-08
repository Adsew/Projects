#include "Core.h"
#include "PointLight.h"

#include "ShaderProgram.h"
#include "GameObject.h"

IMPLEMENT_DYNAMIC_CLASS(PointLight);

PointLight::PointLight()
	: m_constant(0.0f),
	m_linear(0.0f),
	m_quadratic(0.0f)
{
	m_lightType = Light::TYPE::kPoint;
}

PointLight::~PointLight()
{
}

Light::TYPE PointLight::getLightType() const
{
	return m_lightType;
}

void PointLight::initialize()
{
	Light::initialize();
}

void PointLight::load(XMLElement * element)
{
	Light::load(element);

	XMLElement* constant = element->FirstChildElement("constant");
	if (constant != nullptr)
	{
		constant->QueryFloatText(&m_constant);
	}
	XMLElement* linear = element->FirstChildElement("linear");
	if (linear != nullptr)
	{
		linear->QueryFloatText(&m_linear);
	}
	XMLElement* quadratic = element->FirstChildElement("quadratic");
	if (quadratic != nullptr)
	{
		quadratic->QueryFloatText(&m_quadratic);
	}
}

void PointLight::bind(int index, ShaderProgram * program)
{
	Light::bind(index, program);

	if (program == nullptr)
	{
		return;
	}

	std::string obj_name = "lights[" + std::to_string(index) + "]";
	std::string name = obj_name + ".ambient";
	program->bindUniform<const glm::vec3&>(name, m_ambient);

	name = obj_name + ".diffuse";
	program->bindUniform<const glm::vec3&>(name, m_diffuse);

	name = obj_name + ".specular";
	program->bindUniform<const glm::vec3&>(name, m_specular);

	name = obj_name + ".position";
	if (auto go = m_gameObject.lock())
	{
		program->bindUniform<const glm::vec3&>(name, go->transform()->position());
	}

	name = obj_name + ".constant";
	program->bindUniform<float>(name, m_constant);

	name = obj_name + ".linear";
	program->bindUniform<float>(name, m_linear);

	name = obj_name + ".quadratic";
	program->bindUniform<float>(name, m_quadratic);
}
