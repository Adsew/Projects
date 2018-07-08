#pragma once

#include "Component.h"

class PlayerMovement;
class UITextRenderer;

class PlayerTyping : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(PlayerTyping, Component);

private:
    std::weak_ptr<PlayerMovement> pMovement;
    std::weak_ptr<UITextRenderer> pText;
    std::string input;
    bool locked;
public:
    void initialize() override;
    void update(float _deltaTime) override;
    void keyCallback(Window * win, int key, int scancode, int action, int mods);

    std::string &getPlayerName();
};
