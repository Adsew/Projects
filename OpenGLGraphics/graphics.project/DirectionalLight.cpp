#include "Core.h"
#include "DirectionalLight.h"

#include "ShaderProgram.h"

IMPLEMENT_DYNAMIC_CLASS(DirectionalLight);

DirectionalLight::DirectionalLight()
{
	m_lightType = Light::TYPE::kDirectional;
}

void DirectionalLight::initialize()
{
	Light::initialize();
}

void DirectionalLight::update(float _deltaTime)
{
}

void DirectionalLight::load(XMLElement * element)
{
	Light::load(element);

	XMLElement* direction = element->FirstChildElement("direction");
	if (direction != nullptr)
	{
		XMLElement* x = direction->FirstChildElement("x");
		if (x != nullptr) { x->QueryFloatText(&m_direction.x); }
		XMLElement* y = direction->FirstChildElement("y");
		if (y != nullptr) { y->QueryFloatText(&m_direction.y); }
		XMLElement* z = direction->FirstChildElement("z");
		if (z != nullptr) { z->QueryFloatText(&m_direction.z); }
	}
}

void DirectionalLight::bind(int index, ShaderProgram * program)
{
	if (program == nullptr)
	{
		LOG("Light found nullptr ShaderProgram while uploading light data");
		return;
	}

	// bind the light data to the shader
	program->bindUniform<const glm::vec3&>("directional_light.direction", m_direction);
	program->bindUniform<const glm::vec3&>("directional_light.ambient", m_ambient);
	program->bindUniform<const glm::vec3&>("directional_light.diffuse", m_diffuse);
	program->bindUniform<const glm::vec3&>("directional_light.specular", m_specular);
}
