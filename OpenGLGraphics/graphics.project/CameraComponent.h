#pragma once

#include "Component.h"

enum CAMERA_TYPE
{
	kPerspective,
	kOrthographic
};

// GameObject component for the Camera
class CameraComponent : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(CameraComponent, Component);
private:
	float m_fov;
	float m_far;
	float m_near;
	float m_aspect;
	int m_width;
	int m_height;
	bool m_configured;
	bool m_isDirty;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projMatrix;
	CAMERA_TYPE m_type;
	void compute();

public:
	CameraComponent();
	void initialize() override;
	void update(float _deltaTime) override;
	void load(tinyxml2::XMLElement* element) override;

	virtual void calcViewMatrix();
	virtual void calcProjMatrix();

	virtual glm::mat4 calcMVP(const glm::mat4& model_matrix);

	//=========================
	// GETTERS
	//=========================

	float getFOV() const;
	float getFar() const;
	float getNear() const;
	float getAspect() const;
	int getWidth() const;
	int getHeight() const;
	bool isConfigured() const;
	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getProjMatrix() const;

	//=========================
	// SETTERS
	//=========================

	void setFOV(float fov);
	void setFar(float _far);
	void setNear(float _near);
	void setWidth(int width);
	void setHeight(int height);
	void setDimensions(int width, int height);
	void setCameraType(CAMERA_TYPE type);
};