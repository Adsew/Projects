#pragma once

#include "Component.h"

class GameObject;
class ScreenTimerUpdater;
class InventoryRenderer;


class LevelState : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(LevelState, Component);

protected:
    std::weak_ptr<GameObject> player;
    std::weak_ptr<ScreenTimerUpdater> timer;
    std::weak_ptr<InventoryRenderer> inventory;
    bool gameEnded;
    bool firstUpdate;

    LevelState();
    void initialize() override;
    void update(float _deltaTime) override;
    void load(XMLElement* element) override;
public:

};

