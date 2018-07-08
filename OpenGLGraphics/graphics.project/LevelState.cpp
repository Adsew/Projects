#include "Core.h"

#include "GameObjectManager.h"
#include "RenderManager.h"
#include "PlayerTyping.h"
#include "HighScoreLoader.h"
#include "Window.h"
#include "ScreenTimerUpdater.h"
#include "PlayerMovement.h"
#include "InventoryRenderer.h"
#include "UITextRenderer.h"
#include "UIRenderer.h"
#include "GameObject.h"
#include "LevelState.h"

IMPLEMENT_DYNAMIC_CLASS(LevelState);

LevelState::LevelState()
{
    firstUpdate = true;
}

void LevelState::initialize()
{
    player = GameObjectManager::instance().findGameObject("MainCamera");
    if (auto timerGO = GameObjectManager::instance().findGameObject("UI Timer"))
    {
        timer = timerGO->getComponent<ScreenTimerUpdater>();
    }
    if (auto invGO = GameObjectManager::instance().findGameObject("Inventory"))
    {
        inventory = invGO->getComponent<InventoryRenderer>();
    }

    gameEnded = false;
}

void LevelState::update(float _deltaTime)
{
    if (firstUpdate)
    {
        initialize();   // Ensure we find those objects after theyve been created
        firstUpdate = false;
    }
    
    if (!inventory.expired() && !gameEnded)
    {
        if (auto inv = inventory.lock())
        {
            if (inv->panelCount() >= 5)
            {
                gameEnded = true;

                std::string timeTilWon = "";
                std::string pName = "";
                if (!timer.expired())
                {
                    if (auto time = timer.lock())
                    {
                        timeTilWon = time->getTimeAsString();
                        time->stopTiming();
                    }
                }
                if (!player.expired())
                {
                    if (auto p = player.lock())
                    {
                        if (std::shared_ptr<PlayerMovement> movement = p->getComponent<PlayerMovement>())
                        {
                            movement->locked = true;
                        }
                        if (std::shared_ptr<PlayerTyping> name = p->getComponent<PlayerTyping>())
                        {
                            pName = name->getPlayerName();
                        }
                        if (std::shared_ptr<UIRenderer> panel = p->getComponent<UIRenderer>())
                        {
                            panel->setVisible(true);
                        }
                        if (std::shared_ptr<UITextRenderer> text = p->getComponent<UITextRenderer>())
                        {
                            text->setText(std::string(""));
                            text->setPosition(glm::vec2(-0.7f, 0.3f));
                            text->setSize(2);
                            text->setColour(glm::vec3(1.0f, 0.0f, 1.0f));
                            text->setText(std::string("YOU WIN!!\n\nEsc to exit.\n\nYour time:\n") + timeTilWon + "s");

                            if (auto score = getGameObject()->getComponent <HighScoreLoader>())
                            {
                                score->saveHighScore(pName, timeTilWon);
                            }
                        }
                    }
                }
            }
        }
    }
    else if (gameEnded)
    {
        if (glfwGetKey(RenderManager::instance().getWindow()->getWindow(), GLFW_KEY_ENTER) != GLFW_RELEASE)
        {
            // exit game
        }
    }
}

void LevelState::load(XMLElement* element)
{

}
