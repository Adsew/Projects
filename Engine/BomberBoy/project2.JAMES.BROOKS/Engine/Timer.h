#ifndef TIMER_H
#define TIMER_H

/*
Student: James Brooks
Class: Game Architecture

File: Timer.h

Class: Timer

Description: Class to manage time of the game during execution, in delta time
    per update and total play time. Named Timer only to avoid potential conflict
    with anything native named Time.
*/


#include <time.h>

#include "ISystem.h"


class Timer : public ISystem {

    friend class GameEngine;
    friend class FileSystem;


    /***** Variables *****/

private:

    float targetFPS;
    float targetUpdatesPerSecond;

    clock_t prevClockTime;
    
    float runTime;
    float deltaTime;    // In milliseconds

    clock_t logTimerStartTime;
    
    bool logTimerLocked;

    /***** Functions *****/

private:

    Timer() = default;

    Timer(const Timer& other) = default;

    ~Timer() = default;

protected:

    virtual void initialize() override;

    virtual  void clean() override;

    virtual void update() override;

    void beginTiming();

    void setFPS(float f);

public:

    inline static Timer &Instance() {

        static Timer instance;

        return instance;
    }

    // Get the desired fps for other systems to measure by
    float getTargetFPS();

    // Get FPS converted as fraction of seconds
    float getTargetUpdatesPerSecond();

    // Return the delta time of the previous update in milliseconds
    float getDelta();

    // Return the current total game runtime since after initialization in milliseconds
    float getRuntime();

    // Begin timing of a segment and lock the timer in use
    bool startLogTimer();

    // End timing and get the time taken
    float endLogTimer();
};

#endif // !TIMER_H
