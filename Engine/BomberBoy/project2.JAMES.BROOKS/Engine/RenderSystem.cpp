/*
Student: James Brooks
Class: Game Architecture

File: RenderSystem.cpp

Class: RenderSystem

Description: A system responsable for rendering renderable objects to the screen.
*/


#include "Core.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "IRenderable.h"
#include "RenderSystem.h"


RenderSystem::RenderSystem() {

    window = NULL;

    winTitle = "BomberBoy";

    winWidth = 800;
    winHeight = 600;
}

RenderSystem::~RenderSystem() {

    
}

void RenderSystem::initialize() {

    if (winWidth > 0 && winHeight > 0) {

        window = new sf::RenderWindow(sf::VideoMode(winWidth, winHeight), sf::String(winTitle.c_str()));
        
        window->setKeyRepeatEnabled(false);
    }

    renderComponents.resize(PRIORITIES);
}

void RenderSystem::clean() {

    renderComponents.clear();

    window->close();

    delete window;
    window = NULL;
}

void RenderSystem::update() {

    if (window != NULL) {

        window->clear();

        for (int i = LOWEST_PRIORITY; i >= PRIORITIES_START; i--) {

            for (std::list<IRenderable *>::iterator iter = renderComponents[i].begin();
                iter != renderComponents[i].end();
                iter++) {

                (*iter)->render();
            }
        }

        window->display();
    }
}

void RenderSystem::addRenderable(IRenderable *component) {

    if (component != NULL) {

        if (component->priority < PRIORITIES && component->priority >= PRIORITIES_START) {

            renderComponents[component->priority].push_back(component);
        }
        else {

            renderComponents[LOWEST_PRIORITY].push_back(component);
        }
    }
}

void RenderSystem::removeRenderable(IRenderable *component) {
    
    if (component != NULL && renderComponents.size() > 0) {

        if (component->priority < PRIORITIES && component->priority >= PRIORITIES_START) {

            renderComponents[component->priority].remove(component);
        }
        else {

            renderComponents[LOWEST_PRIORITY].remove(component);
        }
    }
}

sf::RenderWindow *RenderSystem::getWindow() {

    return window;
}

void RenderSystem::setWindowSize(int w, int h) {

    if (window != NULL) {
        
        winWidth = w;
        winHeight = h;
        
        delete window;

        window = new sf::RenderWindow(sf::VideoMode(winWidth, winHeight), sf::String(winTitle.c_str()));
    }
}
