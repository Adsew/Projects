#pragma once

#include "Light.h"

class ShaderProgram;

// Holds extended data specific to a directional light
class DirectionalLight : public Light
{
	DECLARE_DYNAMIC_DERIVED_CLASS(DirectionalLight, Light);
private:
	glm::vec3 m_direction;

public:
	DirectionalLight();
	void initialize() override;
	void update(float _deltaTime) override;
	void load(XMLElement* element) override;
	// bind the data of the light to the shader program
	void bind(int index, ShaderProgram* program) override;
};