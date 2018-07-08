#include "Core.h"
#include "CameraComponent.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include <glm/gtc/matrix_transform.hpp>

IMPLEMENT_DYNAMIC_CLASS(CameraComponent);

CameraComponent::CameraComponent()
	: Component(),
	m_fov(0),
	m_far(0),
	m_near(0),
	m_aspect(0),
	m_width(0),
	m_height(0),
	m_configured(false),
	m_isDirty(true),
	m_type(CAMERA_TYPE::kPerspective)
{
}

void CameraComponent::initialize()
{
	Component::initialize();
	compute();
	GameObjectManager::instance().setCamera(getGameObject());
}

void CameraComponent::update(float _deltaTime)
{
	Component::update(_deltaTime);
	compute();
}

void CameraComponent::load(tinyxml2::XMLElement * element)
{
	tinyxml2::XMLElement* fov = element->FirstChildElement("fov");
	if (fov != nullptr)
	{
		fov->QueryFloatText(&m_fov);
	}
	tinyxml2::XMLElement* _far = element->FirstChildElement("far");
	if (_far != nullptr)
	{
		_far->QueryFloatText(&m_far);
	}
	tinyxml2::XMLElement* _near = element->FirstChildElement("near");
	if (_near != nullptr)
	{
		_near->QueryFloatText(&m_near);
	}
	m_width = RenderManager::instance().getWidth();
	m_height = RenderManager::instance().getHeight();
	m_aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
}

void CameraComponent::calcViewMatrix()
{
	if (auto go = m_gameObject.lock())
	{
		m_viewMatrix = glm::lookAt(go->transform()->position(), go->transform()->position() + go->transform()->forward(), go->transform()->up());
	}
}

void CameraComponent::calcProjMatrix()
{
	m_aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

	glViewport(0, 0, static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height));

	switch (m_type)
	{
	case CAMERA_TYPE::kPerspective:
		m_projMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
		break;
	case CAMERA_TYPE::kOrthographic:
		m_projMatrix = glm::ortho(0.0f, static_cast<float>(m_width), 0.0f, static_cast<float>(m_height), m_near, m_far);
		break;
	}
}

glm::mat4 CameraComponent::calcMVP(const glm::mat4 & model_matrix)
{
	return m_projMatrix * m_viewMatrix * model_matrix;
}

float CameraComponent::getFOV() const
{
	return m_fov;
}

float CameraComponent::getFar() const
{
	return m_far;
}

float CameraComponent::getNear() const
{
	return m_near;
}

float CameraComponent::getAspect() const
{
	return m_aspect;
}

int CameraComponent::getWidth() const
{
	return m_width;
}

int CameraComponent::getHeight() const
{
	return m_height;
}

bool CameraComponent::isConfigured() const
{
	return m_configured;
}

const glm::mat4 & CameraComponent::getViewMatrix() const
{
	return m_viewMatrix;
}

const glm::mat4 & CameraComponent::getProjMatrix() const
{
	return m_projMatrix;
}

void CameraComponent::setFOV(float fov)
{
	m_isDirty = true;
	m_fov = fov;
}

void CameraComponent::setFar(float _far)
{
	m_isDirty = true;
	m_far = _far;
}

void CameraComponent::setNear(float _near)
{
	m_isDirty = true;
	m_near = _near;
}

void CameraComponent::setWidth(int width)
{
	m_isDirty = true;
	m_width = width;
}

void CameraComponent::setHeight(int height)
{
	m_isDirty = true;
	m_height = height;
}

void CameraComponent::setDimensions(int width, int height)
{
	m_width = width;
	m_height = height;
	compute();
}

void CameraComponent::setCameraType(CAMERA_TYPE type)
{
	m_isDirty = true;
	m_type = type;
}

void CameraComponent::compute()
{
	calcProjMatrix();
	calcViewMatrix();
}