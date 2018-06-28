/*
Student: James Brooks
Class: Game Architecture

File: GameObject.cpp

Class: GameObject

Description: A standard object to be created into the scene.
*/


#include "Core.h"

#include "Scene.h"
#include "Component.h"
#include "Transform.h"
#include "ComponentManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"


GameObject::GameObject(unsigned int uniqueID)
    : Object(uniqueID) {

    scene = NULL;
    name = "New Game Object";

    transform = (Transform *)ComponentManager::Instance().createComponent("Transform");
    transform->setPosition(0, 0);
    transform->setScale(1.0f, 1.0f);
    transform->setRotation(0);

    objectPoolID = 0;
    isFromObjectPool = false;

    active = true;
    yeilding = false;
}

GameObject::GameObject(unsigned int uniqueID, const char *goName)
    : Object(uniqueID) {

    scene = NULL;
    name = goName;

    transform = (Transform *)ComponentManager::Instance().createComponent("Transform");
    transform->setPosition(0, 0);
    transform->setScale(1.0f, 1.0f);
    transform->setRotation(0);

    objectPoolID = 0;
    isFromObjectPool = false;

    active = true;
    yeilding = false;
}

GameObject::GameObject(unsigned int uniqueID, const std::string &goName)
    : Object(uniqueID) {

    scene = NULL;
    name = goName.c_str();

    transform = (Transform *)ComponentManager::Instance().createComponent("Transform");
    transform->setPosition(0, 0);
    transform->setScale(1.0f, 1.0f);
    transform->setRotation(0);

    active = true;
    yeilding = false;
}

GameObject::~GameObject() {

    transform->destroy();
    transform->gameObject = NULL;
    transform = NULL;

    for (std::map<std::string, Component *>::iterator iter = components.begin();
        iter != components.end();
        iter++) {

        (*iter).second->destroy();
        (*iter).second->gameObject = NULL;
    }

	components.clear();

    if (scene != NULL) {

        scene->unmanageGameObject(this);
    }
}

// Initialize GO to a usable state
void GameObject::initialize() {


}

void GameObject::onDestroy() {


}

// Cycle update updates all game object components
void GameObject::update() {

    if (transform != NULL) {

        transform->update();
    }

    for (std::map<std::string, Component *>::iterator iter = components.begin();
        iter != components.end();
        iter++) {

        (*iter).second->update();
    }
}

void GameObject::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    FileSystem::Instance().getAttribute(accessor, "name", name);

    // Load any components under the game object
    if (FileSystem::Instance().traverseToElement(accessor, "Components")) {
        if (FileSystem::Instance().traverseToChildElement(accessor)) {

            Component *tempComp = NULL;
            std::string type = "";

            do {
                FileSystem::Instance().getAttribute(accessor, "type", type);

                // Special load for transform
                if (type == "Transform") {

                    if (transform == NULL) {

                        transform = (Transform *)ComponentManager::Instance().createComponent("Transform");

                        transform->gameObject = this;
                    }

                    transform->load(accessor);
                }
                // Any other component
                else {

                    tempComp = ComponentManager::Instance().createComponent(type.c_str());

                    if (tempComp != NULL) {

                        addComponent(tempComp);

                        tempComp->load(accessor);
                    }
                }
            } while (FileSystem::Instance().traverseToSyblingElement(accessor));

            FileSystem::Instance().traverseToParentElement(accessor);
        }

        FileSystem::Instance().traverseToParentElement(accessor);
    }
}

// Add a component to the game object for use
void GameObject::addComponent(Component * component) {

    if (component != NULL) {

        std::map<std::string, Component *>::iterator iter = components.find(component->type);

        if (iter == components.end()) {

            components[component->type] = component;
            component->gameObject = this;
        }
    }
}

// Remove the component from the game object, if exists
void GameObject::removeComponent(Component * component) {

    if (component != NULL) {

        std::map<std::string, Component *>::iterator iter = components.find(component->type);

        if (iter != components.end()) {

            (*iter).second->destroy();
            (*iter).second->gameObject = NULL;
            components.erase(iter);
        }
    }
}

// Create a clone of this gameobject with the same components and settings
GameObject *GameObject::clone() {

    GameObject *clone = GameObjectManager::Instance().createGameObject();

    clone->setName(name + " (clone)");

    if (scene != NULL) {

        scene->manageGameObject(clone);
    }
    
    *(clone->transform) = *transform;

    for (std::map<std::string, Component *>::iterator iter = components.begin();
        iter != components.end();
        iter++) {

        Component *newComp = ComponentManager::Instance().createComponent((*iter).second->type.c_str());

        *newComp = *((*iter).second);

        clone->addComponent(newComp);
    }

    return clone;
}

// Enable or disable updating of this game object and all its components
void GameObject::setActive(bool a) {

    active = a;
}

// Check if gameobject is enabled
bool GameObject::isActive() {

    return active;
}

// Sets the game object to no longer update but still render
void GameObject::setYeilding(bool y) {

    yeilding = y;
}

// Check if game object is yeilding
bool GameObject::isYeilding() {

    return yeilding;
}

    /* Gets/Sets */

void GameObject::setName(const char *newName) {

    name = newName;
}

void GameObject::setName(const std::string &newName) {

    this->setName(newName.c_str());
}

std::string &GameObject::getName() {

    return name;
}

// Uses scene of the given game object
void GameObject::setScene(GameObject *go) {

    if (go != NULL) {
        if (go->scene != NULL) {

            if (scene != NULL) {

                scene->unmanageGameObject(this);
            }

            go->scene->manageGameObject(this);
        }
    }
}

Component *GameObject::getComponent(const std::string &type) {

    for (std::map<std::string, Component *>::iterator iter = components.begin();
        iter != components.end();
        iter++) {

        if ((*iter).second->type == type) {

            return (*iter).second;
        }
    }

    return NULL;
}

Transform *GameObject::getTransform() {

    return transform;
}
