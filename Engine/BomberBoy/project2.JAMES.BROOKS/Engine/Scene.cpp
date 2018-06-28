/*
Student: James Brooks
Class: Game Architecture

File: Scene.cpp

Class: Scene

Description: Holds multiple game objects grouped together to make a scene.
Ensures easier management of game objects active at a time.
*/


#include "Core.h"

#include "GameObject.h"
#include "GameObjectManager.h"
#include "Scene.h"



Scene::Scene(unsigned int id, const char *n) : Object(id) {

    name = n;

    gameObjects.clear();
}


Scene::~Scene() {

    for (std::list<GameObject *>::iterator iter = gameObjects.begin();
        iter != gameObjects.end();
        iter++) {

        (*iter)->scene = NULL;
        (*iter)->destroy();
    }

    gameObjects.clear();
}

void Scene::initialize() {


}

void Scene::onDestroy() {


}

// Loads a scene file with the same name as the scene
bool Scene::load() {

    std::unique_ptr<FileSystem::FileAccessor> accessor = FileSystem::Instance().useFile(name);

    if (accessor != NULL) {

        if (FileSystem::Instance().traverseToElement(accessor, "Scene")) {
            if (FileSystem::Instance().traverseToElement(accessor, "GameObject")) {

                do {

                    GameObject *newGO = GameObjectManager::Instance().createGameObject();

                    if (newGO != NULL) {

                        newGO->scene = this;
                        newGO->load(accessor);
                        gameObjects.push_back(newGO);
                    }

                } while (FileSystem::Instance().traverseToSyblingElement(accessor));
            }
        }

        return true;
    }

    return false;
}

// Make the game object part of this scene
void Scene::manageGameObject(GameObject *go) {

    if (go != NULL) {

        go->scene = this;

        gameObjects.push_back(go);
    }
}

// Remove game object from the list of those managed by this scene (does not delete GO)
void Scene::unmanageGameObject(GameObject *go) {

    for (std::list<GameObject *>::iterator iter = gameObjects.begin();
        iter != gameObjects.end();
        iter++) {

        if (*go == *(*iter)) {

            gameObjects.erase(iter);

            break;
        }
    }
}
