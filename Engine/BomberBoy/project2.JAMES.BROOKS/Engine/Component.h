#ifndef COMPONENT_H
#define COMPONENT_H

/*
Student: James Brooks
Class: Game Architecture

File: Component.h

Class: Component

Description: A base class for all components that can be added to a game object.
*/


#include "GameObject.h"
#include "ComponentManager.h"
#include "Object.h"


class Component : public Object {

    friend class ComponentManager;
    friend class GameObject;

    /***** Variables *****/

protected:

    GameObject *gameObject;

public:

    const std::string type;

    
    /***** Functions *****/

protected:
	
    Component(unsigned int uniqueID, const char *compType);

    Component(unsigned int uniqueID, const std::string &compType);

    virtual ~Component();

    // Initialize the component to a usable state
    virtual void initialize() override {}

    // Destroy any data before destruction
    virtual void onDestroy() override {}

    // Cycle update for a component
    virtual void update() = 0;

    // When loading a component from a scene file, details are loaded here
    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) = 0;

public:

        /* Gets/Sets */

    virtual Component &operator=(const Component &comp) = 0;

    GameObject * const getGameObject();
};

#endif // !COMPONENT_H
