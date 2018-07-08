#include "Core.h"

#include "UITextRenderer.h"
#include "ScreenTimerUpdater.h"

IMPLEMENT_DYNAMIC_CLASS(ScreenTimerUpdater);

void ScreenTimerUpdater::initialize()
{
    text = getGameObject()->getComponent<UITextRenderer>();
    gameTime = 0;
    _time = clock();

    keepTiming = true;
}

void ScreenTimerUpdater::update(float _deltaTime)
{
    if (keepTiming)
    {
        _time = clock() - _time;
        gameTime += ((float)_time) / ((clock_t)1000);
        if (!text.expired())
        {
            if (auto textComp = text.lock())
            {
                int flatNumber = (int)gameTime;
                int rounder = (int)(gameTime * 10);
                int decimal = rounder % 10;
                textComp->setText(std::string(std::to_string(flatNumber) + "." + std::to_string(decimal)));
            }
        }
        _time = clock();
    }
}

void ScreenTimerUpdater::stopTiming()
{
    keepTiming = false;
}

float ScreenTimerUpdater::getTime()
{
    return gameTime;
}

std::string ScreenTimerUpdater::getTimeAsString()
{
    int flatNumber = (int)gameTime;
    int rounder = (int)(gameTime * 10);
    int decimal = rounder % 10;
    return std::string(std::to_string(flatNumber) + "." + std::to_string(decimal));
}