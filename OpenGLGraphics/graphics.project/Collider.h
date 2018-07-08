#pragma once

#include "Raycast.h"
#include "Component.h"
class Collider : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Collider, Component);
public:
    // Square dimensions
    glm::vec3 min;
    glm::vec3 max;

    void initialize() override;
    void update(float _deltaTime) override;
    void load(XMLElement* element) override;
    //Check collision
    bool collides(std::shared_ptr<Collider> &other);
    bool collides(Raycast &ray);
};

