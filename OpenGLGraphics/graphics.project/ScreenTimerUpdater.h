#pragma once

#include <time.h>

#include "Component.h"

class UITextRenderer;

class ScreenTimerUpdater : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(ScreenTimerUpdater, Component);
protected:
    std::weak_ptr<UITextRenderer> text;
    float gameTime;
    clock_t _time;
    bool keepTiming;

    void initialize() override;
    void update(float _deltaTime) override;
public:
    void stopTiming();
    float getTime();
    std::string getTimeAsString();
};

