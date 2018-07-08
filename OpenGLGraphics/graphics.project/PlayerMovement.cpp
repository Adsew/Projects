#include "Core.h"
#include "PlayerMovement.h"

#include "HighScoreLoader.h"
#include "Raycast.h"
#include "Collider.h"
#include "SpotLight.h"
#include "CameraComponent.h"
#include "GameObjectManager.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "Window.h"
#include "UITextRenderer.h"
#include "IsPickUpable.h"
#include "InventoryRenderer.h"

#define COLLIDE_DISTANCE_MAX 25.0f

IMPLEMENT_DYNAMIC_CLASS(PlayerMovement);

void PlayerMovement::initialize()
{
	Component::initialize();
	using namespace std::placeholders;
	InputManager::instance().addMouseMoveCallback(std::bind(&PlayerMovement::onCursorPosChange, this, _1, _2, _3));

	locked = true;
}

void PlayerMovement::start()
{
	m_inventory = GameObjectManager::instance().getComponent<InventoryRenderer>();
}

void PlayerMovement::update(float _deltaTime)
{
	if (!locked)
	{
		Component::update(_deltaTime);
		auto window = RenderManager::instance().getWindow();
		if (window == nullptr)
		{
			LOG("PlayerMovement found nullptr window in update()");
			return;
		}

		if (auto go = m_gameObject.lock())
		{
			auto transform = go->transform();
			if (transform == nullptr)
			{
				LOG("PlayerMovement found nullptr transform");
				return;
			}

			if (checkCollisions())
			{
				transform->revertMove();
			}
			else
			{
				if (glfwGetKey(window->getWindow(), GLFW_KEY_W) != GLFW_RELEASE)
				{
					transform->moveForwardFlat(0.1f);
				}
				if (glfwGetKey(window->getWindow(), GLFW_KEY_S) != GLFW_RELEASE)
				{
					transform->moveBackwardFlat(0.1f);
				}
				if (glfwGetKey(window->getWindow(), GLFW_KEY_A) != GLFW_RELEASE)
				{
					transform->strafeLeft(0.1f);
				}
				if (glfwGetKey(window->getWindow(), GLFW_KEY_D) != GLFW_RELEASE)
				{
					transform->strafeRight(0.1f);
				}
			}

			if (glfwGetKey(window->getWindow(), GLFW_KEY_Q) != GLFW_RELEASE)
			{
				transform->rotate(-1.0f, 0.0f, 0.0f);
			}
			if (glfwGetKey(window->getWindow(), GLFW_KEY_E) != GLFW_RELEASE)
			{
				transform->rotate(1.0f, 0.0f, 0.0f);
			}

			// Flashlight on off
			if (glfwGetKey(window->getWindow(), GLFW_KEY_F) != GLFW_RELEASE)
			{
				if (auto light = getGameObject()->getComponent<SpotLight>())
				{
					light->setOn(!light->isOn());
				}
			}

			// Show scores
			if (glfwGetKey(window->getWindow(), GLFW_KEY_TAB) != GLFW_RELEASE)
			{
				if (auto board = getGameObject()->getComponent<UIRenderer>())
				{
					if (!board->isVisible())
					{
						board->setVisible(true);

						if (auto scores = getGameObject()->getComponent<UITextRenderer>())
						{
							scores->setPosition(glm::vec2(-0.72f, 0.46f));
							scores->setColour(glm::vec3(1.0f, 1.0f, 0.0f));
                            if (auto score = getGameObject()->getComponent <HighScoreLoader>())
                            {
                                scores->setText(score->getHighScores());
                            }
						}
					}
					else
					{
						board->setVisible(false);

						if (auto scores = getGameObject()->getComponent<UITextRenderer>())
						{
							scores->setText(std::string(""));
						}
					}

				}
			}

			if (InputManager::instance().isLMousePressed())
			{
				Raycast ray(getGameObject()->getComponent<CameraComponent>());

				std::shared_ptr<GameObject> foundGO = ray.cast(0, 0, 10);

				if (foundGO != nullptr) {
					checkForPickUp(foundGO->getComponent<IsPickUpable>());
				}
			}
		}
	}
}

void PlayerMovement::onCursorPosChange(Window * window, double xPos, double yPos)
{
	if (auto go = m_gameObject.lock())
	{

		auto transform = go->transform();
		if (transform == nullptr)
		{
			LOG("PlayerMovement found nullptr transform");
			return;
		}
		float xOffset = static_cast<float>(xPos) - window->getMidX();
		float yOffset = window->getMidY() - static_cast<float>(yPos);
		transform->rotate(xOffset / 6.0f, yOffset / 6.0f, 0.0f);
	}
}

bool PlayerMovement::checkCollisions()
{
	std::list<std::shared_ptr<Collider>> collidables = GameObjectManager::instance().getComponentsWithType<Collider>();
	std::shared_ptr<Collider> myCollider = getGameObject()->getComponent<Collider>();

	if (myCollider != NULL && collidables.size() > 0)
	{
		// Cull objects not in range to collide
		auto iter = collidables.begin();
		glm::vec3 myPos = getGameObject()->transform()->position();

		while (iter != collidables.end())
		{
			std::shared_ptr<GameObject> go = (*iter)->getGameObject();
			glm::vec3 goPos = go->transform()->position();
			float dist = glm::abs(glm::length(goPos - myPos));

			if (dist > COLLIDE_DISTANCE_MAX || go->getName() == this->getGameObject()->getName())
			{
				auto toRemove = iter;
				iter++;
				collidables.erase(toRemove);
			}
			else {

				iter++;
			}
		}

		for (std::shared_ptr<Collider> col : collidables) {

			if (col->collides(myCollider))
			{
				return true;
			}
		}
	}

	return false;
}

void PlayerMovement::checkForPickUp(std::shared_ptr<IsPickUpable> pickupable)
{
	if (pickupable != nullptr)
	{
		// UI elements
		if (auto inv = m_inventory.lock())
		{
			inv->addInventoryIcon(pickupable->getUUIDHashCode(), pickupable->iconTexture);
		}

		pickupable->getGameObject()->markForDelete();
	}
}
