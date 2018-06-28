/*
Student: James Brooks
Class: Game Architecture

File: IRenderable.cpp

Class: IRenderable

Description: Interface to provide screen rendering functionality.
*/


#include "Core.h"

#include "RenderSystem.h"
#include "IRenderable.h"


IRenderable::IRenderable() {

    priority = LOWEST_PRIORITY;

    RenderSystem::Instance().addRenderable(this);
}

IRenderable::IRenderable(unsigned int p) {

    priority = p;

    RenderSystem::Instance().addRenderable(this);
}

IRenderable::~IRenderable() {

    RenderSystem::Instance().removeRenderable(this);
}

// Set the priority the object will render over others
void IRenderable::setRenderPriority(unsigned int p) {

    if (p >= 0 && p < PRIORITIES) {

        RenderSystem::Instance().removeRenderable(this);

        priority = p;

        RenderSystem::Instance().addRenderable(this);
    }
}

// Get the priority the object will render over others
unsigned int IRenderable::getRenderPriority() const {

    return priority;
}
