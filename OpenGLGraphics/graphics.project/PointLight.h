#pragma once

#include "Light.h"

// point class holds specific light data
class PointLight : public Light
{
	DECLARE_DYNAMIC_DERIVED_CLASS(PointLight, Light);
protected:
	float m_constant;
	float m_linear;
	float m_quadratic;

public:
	PointLight();
	~PointLight();
	TYPE getLightType() const;
	void initialize() override;
	void load(XMLElement* element) override;
	void bind(int index, ShaderProgram* program) override;
};