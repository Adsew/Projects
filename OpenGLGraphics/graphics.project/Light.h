#pragma once

#include "Component.h"

class ShaderProgram;

// parent class of all lights in the window
class Light : public Component
{
	DECLARE_ABSTRACT_DERIVED_CLASS(Light, Component);
public:
	enum TYPE
	{
		kDirectional,
		kPoint,
		kSpot,
		kLastType
	};
protected:

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;


	TYPE m_lightType;

public:
	Light();
	virtual ~Light();
	TYPE getLightType() const;
	void initialize() override;
	void update(float _deltaTime) override;
	void load(XMLElement* element) override;
	virtual void bind(int index, ShaderProgram* program);
};