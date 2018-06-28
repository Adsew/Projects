/*
Student: James Brooks
Class: Game Architecture

File: Timer.cpp

Class: Timer

Description: Class to manage time of the game during execution, in delta time
    per update and total play time. Named Timer only to avoid potential conflict
    with anything native named Time.
*/


#include "Core.h"

#include "Timer.h"


void Timer::initialize() {

    targetFPS = 30.0f;
    runTime = 0.0f;
    deltaTime = 0.0f;
}

void Timer::clean() {


}

void Timer::update() {

    prevClockTime = clock() - prevClockTime;
    deltaTime = ((float)prevClockTime) / ((clock_t)1000.0f);
    runTime += deltaTime;

    prevClockTime = clock();
}

void Timer::beginTiming() {

    prevClockTime = clock();
}

void Timer::setFPS(float f) {

    targetFPS = f;
    targetUpdatesPerSecond = 1.0f / targetFPS;
}

// Get the desired fps for other systems to measure by
float Timer::getTargetFPS() {

    return targetFPS;
}

// Get FPS converted as fraction of seconds
float Timer::getTargetUpdatesPerSecond() {

    return targetUpdatesPerSecond;
}

// Return the delta time of the previous update
float Timer::getDelta() {

    return deltaTime;
}

// Return the current total game runtime since after initialization
float Timer::getRuntime() {

    return runTime;
}

// Begin timing of a segment and lock the timer in use
bool Timer::startLogTimer() {

    if (!logTimerLocked) {

        logTimerLocked = true;

        logTimerStartTime = clock();

        return true;
    }

    return false;
}

// End timing and get the time taken
float Timer::endLogTimer() {

    if (logTimerLocked) {

        logTimerLocked = false;

        return ( (float)(clock() - logTimerStartTime) ) / ((clock_t)1000.0f);
    }

    return 0.0f;
}
