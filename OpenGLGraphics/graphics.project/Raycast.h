#pragma once

#include "GameObject.h"
#include "CameraComponent.h"

// A raycast checks for colliders in its path
class Raycast
{
private:
    std::shared_ptr<CameraComponent> camera;

public:
    glm::vec3 normal;
    glm::vec3 ray;
    glm::vec3 origin;
    glm::vec3 end;

    Raycast(std::shared_ptr<CameraComponent> cam);
    ~Raycast();
    std::shared_ptr<GameObject> cast(float xMouse, float yMouse, float dist);
};

