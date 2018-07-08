#pragma once

#include "Component.h"

#include <glm\glm.hpp>

// Transform class for moving GameObjects about the world
class Transform : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Transform, Component);
private:
	std::list<glm::vec3> moveHistory;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_mvMatrix;
	glm::mat4 m_worldMatrix;
	glm::vec3 m_target;
	glm::vec3 m_position;

	float m_yaw;
	float m_pitch;
	float m_roll;

	glm::vec3 m_scale;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;

	bool b_dirty;

	void internalUpdate();
	void updateVectors(bool constrain);

public:
	Transform();
	virtual ~Transform();

	const glm::vec3& getTarget() const;

	void initialize() override;
	void update(float _deltaTime) override;
	void load(XMLElement* element) override;
	bool isDirty() const;
	void setPosition(const glm::vec3& vec);
	void setPosition(float x, float y, float z);
	void setRight(const glm::vec3& vec);
	void setForward(const glm::vec3& vec);
	void setUp(const glm::vec3& vec);
	void moveForward(float _step);
	void moveBackward(float _step);
	void moveForwardFlat(float _step);
	void moveBackwardFlat(float _step);
	void revertMove();
	void strafeRight(float _step);
	void strafeLeft(float _step);
	void translate(float x, float y, float z);
	void translate(const glm::vec3& v);
	void scale(float x, float y, float z);
	void scale(const glm::vec3& v);
	void rotate(float deg, const glm::vec3& v, bool constrainPitch = true);
	void rotate(float deg, int x, int y, int z, bool constrainPitch = true);
	void rotate(float yaw, float pitch, float roll, bool constrainPitch = true);
	void setRotation(float deg, const glm::vec3& v, bool constrainPitch = true);
	void setRotation(float deg, int x, int y, int z, bool constrainPitch = true);
	const glm::mat4& getWorldMatrix() const;
	const glm::mat4& getModelMatrix() const;
	const glm::mat4& getMVMatrix() const;
	const glm::vec3& position() const;
	const glm::vec3& up() const;
	const glm::vec3& right() const;
	const glm::vec3& forward() const;

	void setYaw(float yaw);
	void setPitch(float pitch);
	void setRoll(float roll);
};

