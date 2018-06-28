#ifndef TRANSFORM_H
#define TRANSFORM_H

/*
Student: James Brooks
Class: Game Architecture

File: Transform.h

Class: Transform

Description: Holds the position, orientation, and scale data for a game object.
*/


#include <SFML/Graphics.hpp>

#include "Component.h"


class Transform : public Component {

    REGISTER_COMPONENT(Transform)


    /***** Variables *****/

public:

    sf::Vector2f position;
    sf::Vector2f scale;
    float rotation;


    /***** Functions *****/

protected:

    virtual void initialize() override;

    virtual void update() override;

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

public:

    virtual Component &operator=(const Component &comp) override;

    Transform &operator=(const Transform &trans);

        /* Sets/Gets */

    void setPosition(float x, float y);

    void setScale(float x, float y);

    void setRotation(float r);
};


#endif // !TRANSFORM_H
