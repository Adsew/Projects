#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

/*
Student: James Brooks
Class: Game Architecture

File: AnimatedSprite.h

Class: AnimatedSprite

Description: A small image to be rendered to the screen. Animated over time.
*/


#include "Sprite.h"


class AnimatedSprite : public Sprite {

    REGISTER_COMPONENT(AnimatedSprite);


    /***** Variables *****/

protected:

    std::vector<int *> animations;      // Holds width, height, xOrigin, yOrigin, steps in that order
    int animationCount;                 // Total animations
    int currentAnim;                    // Currently playing animation
    int step;                           // Current step in the animation
    int playCount;                      // Number of times to play current animation
    int numTimesPlayed;                 // Number of times animation has played

    float deltaTime;

    /***** Functions *****/

protected:

    virtual void initialize() override;

    virtual void onDestroy() override;

    virtual void update() override;

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

public:

    virtual Component &operator=(const Component &comp) override;

    // Set which animation is currently playing from the sprite sheet
    bool setAnimation(int animID);

    // Reset the current animation back to the first frame
    void resetAnimation();

    // Return the number of animations loaded from the sprite
    int getAnimationCount();

    // Set the number of times to play the current animation
    // Infinite if not set after setAnimation call
    void setPlayCount(int numTimes);

    // True if animation has played the set number of times by setPlayCount
    bool hasPlayedCountTimes();
};

#endif // !ANIMATED_SPRITE_H
