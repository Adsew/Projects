#include "Core.h"
#include "Transform.h"

#include <glm\gtc\matrix_transform.hpp>

#include "CameraComponent.h"
#include "GameObjectManager.h"
#include "GameObject.h"

IMPLEMENT_DYNAMIC_CLASS(Transform);

Transform::Transform()
	: Component(),
	m_modelMatrix(1.0f),
	m_worldMatrix(1.0f),
	m_position(glm::vec3(0, 0, 0)),
	m_yaw(0),
	m_pitch(),
	m_roll(0),
	m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
	m_up(glm::vec3(0, 1, 0)),
	m_target(glm::vec3(0, 0, -1)),
	m_forward(glm::vec3(0, 0, -1)),
	b_dirty(false)
{
	updateVectors(true);
}

Transform::~Transform()
{
}

const glm::vec3 & Transform::getTarget() const
{
	return m_target;
}

void Transform::internalUpdate()
{
	glm::mat4 mat_trans = glm::translate(glm::mat4(1.0f), m_position);
	glm::mat4 mat_scale = glm::scale(glm::mat4(1.0f), m_scale);

	float radX = glm::radians(m_yaw);
	float radY = glm::radians(m_pitch);
	float radZ = glm::radians(m_roll);

	glm::mat4 mat_rot_x = glm::rotate(glm::mat4(1.0f), radX, glm::vec3(1, 0, 0));
	glm::mat4 mat_rot_y = glm::rotate(glm::mat4(1.0f), radY, glm::vec3(0, 1, 0));
	glm::mat4 mat_rot_z = glm::rotate(glm::mat4(1.0f), radZ, glm::vec3(0, 0, 1));

	glm::mat4 mat_rot = mat_rot_x * mat_rot_y * mat_rot_z;

	m_modelMatrix = mat_trans * mat_rot * mat_scale;

	b_dirty = true;
}

void Transform::updateVectors(bool constrain)
{
	if (constrain)
	{
		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
		else if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}
	}

	float radYaw = glm::radians(m_yaw);
	float radPitch = glm::radians(m_pitch);
	float radRoll = glm::radians(m_roll);

	m_forward.x = cosf(radYaw) * cosf(radPitch);
	m_forward.y = sinf(radPitch);
	m_forward.z = sinf(radYaw) * cosf(radPitch);

	m_target = m_position + glm::normalize(m_forward);

	m_right = glm::normalize(glm::cross(m_forward, m_up));
	//m_up = glm::normalize(glm::cross(m_right, m_forward));
}

void Transform::initialize()
{
	Component::initialize();
}

void Transform::update(float _deltaTime)
{
	Component::update(_deltaTime);
	b_dirty = false;
	auto camera = GameObjectManager::instance().getCamera();
	if (camera == nullptr)
	{
		LOG("Unable to find camera in scene");
		return;
	}

	auto cc = camera->getComponent<CameraComponent>();
	if (cc == nullptr)
	{
		std::cout << "Could not find camera component in Transform" << std::endl;
		return;
	}

	m_worldMatrix = cc->calcMVP(m_modelMatrix);
	m_mvMatrix = cc->getViewMatrix() * m_modelMatrix;

	if (moveHistory.size() > 15)
	{
		moveHistory.pop_front();
	}
}

void Transform::load(XMLElement * element)
{
	Component::load(element);
	XMLElement* data = element->FirstChildElement("yaw");
	if (data != nullptr)
	{
		data->QueryFloatText(&m_yaw);
	}

	data = element->FirstChildElement("pitch");
	if (data != nullptr)
	{
		data->QueryFloatText(&m_pitch);
	}

	data = element->FirstChildElement("roll");
	if (data != nullptr)
	{
		data->QueryFloatText(&m_roll);
	}

	data = element->FirstChildElement("position");
	if (data != nullptr)
	{
		// grab each component from position
		XMLElement* comp = data->FirstChildElement("x");
		if (comp != nullptr)
		{
			comp->QueryFloatText(&m_position.x);
		}

		comp = data->FirstChildElement("y");
		if (comp != nullptr)
		{
			comp->QueryFloatText(&m_position.y);
		}

		comp = data->FirstChildElement("z");
		if (comp != nullptr)
		{
			comp->QueryFloatText(&m_position.z);
		}
	}

	data = element->FirstChildElement("scale");
	if (data != nullptr)
	{
		// grab each component of scale
		XMLElement* comp = data->FirstChildElement("x");
		if (comp != nullptr)
		{
			comp->QueryFloatText(&m_scale.x);
		}

		comp = data->FirstChildElement("y");
		if (comp != nullptr)
		{
			comp->QueryFloatText(&m_scale.y);
		}

		comp = data->FirstChildElement("z");
		if (comp != nullptr)
		{
			comp->QueryFloatText(&m_scale.z);
		}
	}

	// calculate resulting vectors
	updateVectors(false);	// do not constrain. allow components to decide that
	internalUpdate();		// compute model matrix
}

bool Transform::isDirty() const {

	return b_dirty;
}

void Transform::setPosition(const glm::vec3 & vec)
{
	m_position = vec;
	internalUpdate();
}

void Transform::setPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	internalUpdate();
}

void Transform::setRight(const glm::vec3 & vec)
{
	m_right = vec;
	internalUpdate();
}

void Transform::setForward(const glm::vec3 & vec)
{
	m_forward = vec;
	internalUpdate();
}

void Transform::setUp(const glm::vec3 & vec)
{
	m_up = vec;
	internalUpdate();
}

void Transform::moveForward(float _step)
{
	m_position += m_forward * _step;
	moveHistory.push_back(m_position);
	m_target = m_position + m_forward;
	internalUpdate();
}

void Transform::moveBackward(float _step)
{
	m_position -= m_forward * _step;
	moveHistory.push_back(m_position);
	m_target = m_position + m_forward;
	internalUpdate();
}

void Transform::moveForwardFlat(float _step)
{
	glm::vec3 flat_forward = m_forward;
	flat_forward.y = 0;
	flat_forward = glm::normalize(flat_forward);
	m_position += flat_forward * _step;
	moveHistory.push_back(m_position);
	m_target = m_position + m_forward;
	internalUpdate();
}

void Transform::moveBackwardFlat(float _step)
{
	glm::vec3 flat_forward = m_forward;
	flat_forward.y = 0;
	flat_forward = glm::normalize(flat_forward);
	m_position -= flat_forward * _step;
	moveHistory.push_back(m_position);
	m_target = m_position + m_forward;
	internalUpdate();
}

void Transform::revertMove()
{
	if (moveHistory.size() > 1)
	{
		moveHistory.pop_back();
		m_position = moveHistory.back();
		m_target = m_position + m_forward;
		internalUpdate();
	}
}

void Transform::strafeRight(float _step)
{
	m_position += m_right * _step;
	m_target = m_position + m_forward;
	internalUpdate();
}

void Transform::strafeLeft(float _step)
{
	m_position -= m_right * _step;
	m_target = m_position + m_forward;
	internalUpdate();
}

void Transform::translate(float x, float y, float z)
{
	m_position += glm::vec3(x, y, z);
	m_target = m_position + m_forward;
	internalUpdate();
}

void Transform::translate(const glm::vec3 & v)
{
	m_position += v;
	m_target = m_position + m_forward;
	internalUpdate();
}

void Transform::scale(float x, float y, float z)
{
	m_scale = glm::vec3(x, y, z);
	internalUpdate();
}

void Transform::scale(const glm::vec3 & v)
{
	m_scale = v;
	internalUpdate();
}

void Transform::rotate(float deg, const glm::vec3 & v, bool constrainPitch)
{
	rotate(deg, static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z), constrainPitch);
}

void Transform::rotate(float deg, int x, int y, int z, bool constrainPitch)
{
	if (x == 1)
	{
		m_yaw += deg;
	}
	else if (y == 1)
	{
		m_pitch += deg;
	}
	else if (z == 1)
	{
		m_roll += deg;
	}
	updateVectors(constrainPitch);
	internalUpdate();
}

void Transform::rotate(float yaw, float pitch, float roll, bool constrainPitch)
{
	m_yaw += yaw;
	m_pitch += pitch;
	m_roll += roll;
	updateVectors(constrainPitch);
	internalUpdate();
}

void Transform::setRotation(float deg, const glm::vec3 & v, bool constrainPitch)
{
	setRotation(deg, static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z), constrainPitch);
}

void Transform::setRotation(float deg, int x, int y, int z, bool constrainPitch)
{
	if (x == 1)
	{
		m_yaw = deg;
	}
	else if (y == 1)
	{
		m_pitch = deg;
	}
	else if (z == 1)
	{
		m_roll = deg;
	}
	updateVectors(constrainPitch);
	internalUpdate();
}

const glm::mat4 & Transform::getWorldMatrix() const
{
	return m_worldMatrix;
}

const glm::mat4 & Transform::getModelMatrix() const
{
	return m_modelMatrix;
}

const glm::mat4 & Transform::getMVMatrix() const
{
	return m_mvMatrix;
}

const glm::vec3 & Transform::position() const
{
	return m_position;
}

const glm::vec3 & Transform::up() const
{
	return m_up;
}

const glm::vec3 & Transform::right() const
{
	return m_right;
}

const glm::vec3 & Transform::forward() const
{
	return m_forward;
}

void Transform::setYaw(float yaw)
{
	m_yaw = yaw;
	updateVectors(true);
}

void Transform::setPitch(float pitch)
{
	m_pitch = pitch;
	updateVectors(true);
}

void Transform::setRoll(float roll)
{
	m_roll = roll;
	updateVectors(true);
}