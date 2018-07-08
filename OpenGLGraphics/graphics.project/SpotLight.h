#pragma once

#include "PointLight.h"

class SpotLight : public PointLight
{
    DECLARE_DYNAMIC_DERIVED_CLASS(SpotLight, PointLight);
protected:
    float m_cutoffAngle;
    float m_outCutoffAngle;
    bool m_on;
public:
    SpotLight();
    ~SpotLight();
    void initialize() override;
    void load(XMLElement* element) override;
    void bind(int index, ShaderProgram* program) override;
    void setCuttofAngle(float i, float o);
    void setOn(bool o);
    bool isOn();
};

