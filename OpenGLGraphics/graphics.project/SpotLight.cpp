#include "Core.h"

#include "ShaderProgram.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "SpotLight.h"

IMPLEMENT_DYNAMIC_CLASS(SpotLight);

SpotLight::SpotLight()
    :
    m_cutoffAngle(45),
    m_outCutoffAngle(50),
    m_on(true)
{
    m_lightType = Light::TYPE::kSpot;
}

SpotLight::~SpotLight()
{
}

void SpotLight::initialize()
{
    PointLight::initialize();
}

void SpotLight::load(XMLElement* element)
{
    PointLight::load(element);

    XMLElement* node = element->FirstChildElement("cutoff");
    if (node != nullptr)
    {
        node->QueryFloatText(&m_cutoffAngle);
    }
    node = element->FirstChildElement("outcutoff");
    if (node != nullptr)
    {
        node->QueryFloatText(&m_outCutoffAngle);
    }
}

void SpotLight::bind(int index, ShaderProgram* program)
{
    if (program == nullptr)
    {
        return;
    }

    std::string obj_name = "flashlight";

    std::string name = obj_name + ".ambient";
    program->bindUniform<const glm::vec3&>(name, m_ambient);

    name = obj_name + ".diffuse";
    program->bindUniform<const glm::vec3&>(name, m_diffuse);

    name = obj_name + ".specular";
    program->bindUniform<const glm::vec3&>(name, m_specular);

    
    if (auto go = m_gameObject.lock())
    {
        name = obj_name + ".position";
        program->bindUniform<const glm::vec3&>(name, go->transform()->position());

        name = obj_name + ".direction";
        program->bindUniform<const glm::vec3&>(name, go->transform()->forward());
    }


    name = obj_name + ".constant";
    program->bindUniform<float>(name, m_constant);

    name = obj_name + ".linear";
    program->bindUniform<float>(name, m_linear);

    name = obj_name + ".quadratic";
    program->bindUniform<float>(name, m_quadratic);


    name = obj_name + ".cutoff";
    program->bindUniform<float>(name, glm::cos(glm::radians(m_cutoffAngle)));

    name = obj_name + ".outerCutoff";
    program->bindUniform<float>(name, glm::cos(glm::radians(m_outCutoffAngle)));

    name = obj_name + ".on";
    program->bindUniform<bool>(name, m_on);
}

void SpotLight::setCuttofAngle(float i, float o)
{
    m_cutoffAngle = i;
    m_outCutoffAngle = o;
}

void SpotLight::setOn(bool o)
{
    m_on = o;
}

bool SpotLight::isOn()
{
    return m_on;
}
