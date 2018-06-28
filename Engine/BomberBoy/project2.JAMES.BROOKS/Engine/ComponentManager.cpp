/*
Student: James Brooks
Class: Game Architecture

File: ComponentManager.cpp

Class: ComponentManager

Description: Maintains the memory and functionality of all components currently loaded.
*/


#include "Core.h"
#include "TypeRegisterRTTI.h"

#include "Component.h"
#include "ComponentManager.h"


ComponentManager::~ComponentManager() {

    for (std::list<Component *>::iterator iter = components.begin();
        iter != components.end();
        iter++) {

        delete *iter;
    }

    components.clear();
}

void ComponentManager::initialize() {
    
    REGISTER_CORE_COMPONENTS();
}

void ComponentManager::clean() {

    for (std::list<Component *>::iterator iter = components.begin();
        iter != components.end();
        iter++) {

        (*iter)->onDestroy();
        delete *iter;
    }

    components.clear();
}

void ComponentManager::update() {

    std::list<Component *>::iterator iter = components.begin();
    
    while (iter != components.end()) {

        if ((*iter)->pollNeedsDeletion()) {

            std::list<Component *>::iterator temp = iter;

            iter++;

            (*temp)->onDestroy();

            delete *temp;
            components.erase(temp);
        }
        else {

            iter++;
        }
    }
}

void ComponentManager::addComponentType(const char *name, std::function<Component *(unsigned int)> creationFunc) {

    if (createComponentFuncs.find(name) == createComponentFuncs.end()) {

        createComponentFuncs[name] = creationFunc;
    }
}

Component *ComponentManager::createComponent(const char *compType) {

    try {

        Component *tempComp = createComponentFuncs.at(compType)(Object::generateID());

        if (tempComp != NULL) {

            tempComp->initialize();
            components.push_back(tempComp);

            return tempComp;
        }
    }
    catch (...) {}

    return NULL;
}
