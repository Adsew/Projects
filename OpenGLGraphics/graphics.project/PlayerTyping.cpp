#include "Core.h"

#include "Window.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "UITextRenderer.h"
#include "PlayerMovement.h"
#include "PlayerTyping.h"

#define INPUT_PAUSE 0.3f

IMPLEMENT_DYNAMIC_CLASS(PlayerTyping);

void PlayerTyping::initialize()
{
    using namespace std::placeholders;

    pMovement = getGameObject()->getComponent<PlayerMovement>();
    pText = getGameObject()->getComponent<UITextRenderer>();

    InputManager::instance().addKeyCallback(std::bind(&PlayerTyping::keyCallback, this, _1, _2, _3, _4, _5));

    locked = false;
}

void PlayerTyping::update(float _deltaTime)
{
    
}

void PlayerTyping::keyCallback(Window * win, int key, int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE && !locked)
    {
        // On enter, end name mode
        if (key == 257)
        {
            if (!pMovement.expired() && input.size() > 0)
            {
                if (auto m = pMovement.lock())
                {
                    m->locked = false;
                    if (!pText.expired())
                    {
                        if (auto t = pText.lock())
                        {
                            t->setPosition(glm::vec2(-0.95f, -0.9f));
                            t->setText(input);
                        }
                    }
                    getGameObject()->removeComponent(getUUIDHashCode());
                    locked = true;
                }
            }
        }
        // Backspace
        else if (key == 259)
        {
            if (input.size() > 0)
            {
                input.resize(input.size() - 1);

                if (!pText.expired())
                {
                    if (auto t = pText.lock())
                    {
                        t->setText("Name\n-----\n" + input);
                    }
                }
            }
        }
        // Else type valid keys
        else if (key >= 32 && key <= 126)
        {
            input += (char)key;

            if (!pText.expired())
            {
                if (auto t = pText.lock())
                {
                    t->setText("Name\n-----\n" + input);
                }
            }
        }
    }
}

std::string &PlayerTyping::getPlayerName()
{
    return input;
}

