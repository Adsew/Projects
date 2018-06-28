/*
Student: James Brooks
Class: Game Architecture

File: Transform.cpp

Class: Transform

Description: Holds the position, orientation, and scale data for a game object.
*/


#include "Core.h"

#include "Transform.h"


IMPLEMENT_COMPONENT(Transform);


void Transform::initialize() {

    position.x = 0;
    position.y = 0;
    scale.x = 0;
    scale.y = 0;
    rotation = 0;
}

void Transform::update() {


}

void Transform::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    FileSystem::Instance().getAttribute(accessor, "position.x", position.x);
    FileSystem::Instance().getAttribute(accessor, "position.y", position.y);
    FileSystem::Instance().getAttribute(accessor, "scale.x", scale.x);
    FileSystem::Instance().getAttribute(accessor, "scale.y", scale.y);
    FileSystem::Instance().getAttribute(accessor, "rotation", rotation);
}

Component &Transform::operator=(const Component &comp) {

    const Transform *orig = (const Transform *)&comp;

    position.x = orig->position.x;
    position.y = orig->position.y;
    scale.x = orig->scale.x;
    scale.y = orig->scale.y;
    rotation = orig->rotation;

    return *this;
}

Transform &Transform::operator=(const Transform &trans) {

    position.x = trans.position.x;
    position.y = trans.position.y;
    scale.x = trans.scale.x;
    scale.y = trans.scale.y;
    rotation = trans.rotation;

    return *this;
}

void Transform::setPosition(float x, float y) {

    position.x = x;
    position.y = y;
}

void Transform::setScale(float x, float y) {

    scale.x = x;
    scale.y = y;
}

void Transform::setRotation(float r) {

    rotation = r;
}
