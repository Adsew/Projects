#include "Core.h"

#include "GameObjectManager.h"
#include "Transform.h"
#include "Collider.h"
#include "Raycast.h"


Raycast::Raycast(std::shared_ptr<CameraComponent> cam)
{
	camera = cam;
}


Raycast::~Raycast()
{
}

std::shared_ptr<GameObject> Raycast::cast(float xMouse, float yMouse, float dist)
{
	if (camera != NULL)
	{
 		std::shared_ptr<Transform> trans = camera->getGameObject()->transform();
		origin = trans->position();

		// Create the normal of the cast
		glm::vec4 castDir(xMouse, yMouse, -1.0f, 1.0f);                         // Camera clip space normal (view space vector)
		glm::vec4 castEye = glm::inverse(camera->getProjMatrix()) * castDir;    // Undo projection
		castEye = glm::vec4(castEye.x, castEye.y, -1.0f, 0.0f);                 // Get x and y to point "true forward"
		glm::vec3 worldCast = glm::inverse(camera->getViewMatrix()) * castEye;  // Convert to world coordinates
		worldCast = glm::normalize(worldCast);                                  // Should be 0, 0, -1 if mouse is in middle of screen

		normal = worldCast;
		ray = worldCast * dist;
		end = origin + ray;

		// Cull the list of game objects to only those that
		// could potentially collide with the raycast
        // and find closest game object on the ray
		std::list<std::shared_ptr<Collider>> objectColliders = GameObjectManager::instance().getComponentsWithType<Collider>();
        std::pair<float, std::shared_ptr<GameObject>> closest(-1, NULL);

        for (auto iter = objectColliders.begin(); iter != objectColliders.end();) {

            glm::vec3 dirToCollidable = (*iter)->getGameObject()->transform()->position() - origin;

            // Eliminate self
            if ((*iter)->getGameObject() == camera->getGameObject())
            {
                objectColliders.erase(iter++);
            }
            // Eliminate things behind player
            else if (glm::dot(ray, dirToCollidable) < 0)
            {
                objectColliders.erase(iter++);
            }
            else
            {
                if ((*iter)->collides(*this))
                {
                    // None set yet
                    if (closest.second == NULL) {

                        closest.first = glm::abs(glm::length(dirToCollidable));
                        closest.second = (*iter)->getGameObject();
                    }
                    // Take closer of two
                    else
                    {
                        float length = glm::abs(glm::length(dirToCollidable));
                        if (length < closest.first) {

                            closest.first = length;
                            closest.second = (*iter)->getGameObject();
                        }
                    }
                }
                ++iter;
            }
        }

        return closest.second;
	}

	return NULL;
}