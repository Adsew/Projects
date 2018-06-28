/*
Student: James Brooks
Class: Game Architecture

File: GameObjectManager.cpp

Class: GameObjectManager

Description: Maintains the memory and funcitonality of all game objects currently loaded.
*/


#include "Core.h"

#include "GameObject.h"
#include "GameObjectManager.h"


GameObjectManager::~GameObjectManager() {

    for (std::list<GameObject *>::iterator iter = recentlyCreated.begin();
        iter != recentlyCreated.end();
        iter++) {

        delete *iter;
    }

    recentlyCreated.clear();

    for (std::list<GameObject *>::iterator iter = gameObjects.begin();
        iter != gameObjects.end();
        iter++) {

        delete *iter;
    }

    gameObjects.clear();
}

void GameObjectManager::initialize() {

    Object::initializeIDs();

    poolIDCount = 0;
}

void GameObjectManager::clean() {

    for (std::list<GameObject *>::iterator iter = recentlyCreated.begin();
        iter != recentlyCreated.end();
        iter++) {

        delete *iter;
    }

    recentlyCreated.clear();

    for (std::list<GameObject *>::iterator iter = gameObjects.begin();
        iter != gameObjects.end();
        iter++) {

        delete *iter;
    }

    gameObjects.clear();
}

void GameObjectManager::update() {

    std::list<GameObject *>::iterator iter;

    // Add new gameobjects to this cycle update
    for (iter = recentlyCreated.begin();
        iter != recentlyCreated.end();
        iter++) {

        gameObjects.push_back(*iter);
    }

    recentlyCreated.clear();

    // Deletion pass
    iter = gameObjects.begin();

    while (iter != gameObjects.end()) {

        if ((*iter)->pollNeedsDeletion()) {

            std::list<GameObject *>::iterator temp = iter;

            iter++;

            if ((*temp)->isFromObjectPool) {

                returnToPool((*temp)->objectPoolID, (*temp));
            }
            else {

                delete *temp;
                gameObjects.erase(temp);
            }
        }
        else {

            iter++;
        }
    }

    // Normal update after
    for (iter = gameObjects.begin();
        iter != gameObjects.end();
        iter++) {

        if ((*iter)->active && !(*iter)->isYeilding()) {

            (*iter)->update();
        }
    }
}

// Create an empty game object
GameObject *GameObjectManager::createGameObject() {

    GameObject *go = new GameObject(Object::generateID());
    
    recentlyCreated.push_back(go);

    return go;
}

// Creates a game object using a prefab file, returns null if none found
GameObject *GameObjectManager::createGameObjectFromPrefab(const char *prefab) {

    std::unique_ptr<FileSystem::FileAccessor> accessor = FileSystem::Instance().useFile(prefab);

    if (accessor != NULL) {

        FileSystem::Instance().traverseToElement(accessor, "GameObject");

        GameObject *go = new GameObject(Object::generateID());

        go->load(accessor);

        recentlyCreated.push_back(go);

        return go;
    }

    return NULL;
}

// Find game object by given name
GameObject *GameObjectManager::findGameObjectByName(const char *name) {

    std::string goName = name;

    for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++) {

        if ((*iter)->getName() == goName) {

            return *iter;
        }
    }

    return NULL;
}

    /* Object Pool Functions */

// Creates a pool of the given game object, returns id to access pool
// Does not take responsability for the sample game objects
// Returns -1 on error
unsigned int GameObjectManager::createObjectPool(GameObject *sample, const unsigned int amount) {

    if (sample != NULL && amount > 0) {

        objectPool[poolIDCount].resize(amount);
        objectPoolInUseMap[poolIDCount].resize(amount);

        for (unsigned int i = 0; i < objectPool[poolIDCount].size(); i++) {

            objectPool[poolIDCount][i] = sample->clone();
            objectPool[poolIDCount][i]->setActive(false);
            objectPool[poolIDCount][i]->objectPoolID = poolIDCount;
            objectPool[poolIDCount][i]->isFromObjectPool = true;

            objectPoolInUseMap[poolIDCount][i] = false;
        }

        poolIDCount++;

        return poolIDCount - 1;
    }

    return -1;
}

// Request a game object from the pool
// NULL if none available or pool doesn't exist
GameObject *GameObjectManager::requestFromPool(const unsigned int id) {

    if (objectPool.find(id) != objectPool.end()) {

        for (unsigned int i = 0; i < objectPool[id].size(); i++) {

            if (!objectPoolInUseMap[id][i]) {

                objectPoolInUseMap[id][i] = true;

                objectPool[id][i]->setActive(true);

                return objectPool[id][i];
            }
        }
    }

    return NULL;
}

// Return an object to the pool for later use
void GameObjectManager::returnToPool(const unsigned int id, GameObject *object) {

    if (objectPool.find(id) != objectPool.end()) {

        for (unsigned int i = 0; i < objectPool[id].size(); i++) {

            // Check if pointer is in the pool
            if (objectPool[id][i] == object) {

                objectPoolInUseMap[id][i] = false;

                objectPool[id][i]->setActive(false);

                objectPool[id][i]->undestroy();

                break;
            }
        }
    }
}

// Dispose of the object pool, deleting all objects in the pool
void GameObjectManager::destroyObjectPool(const unsigned int id) {

    if (objectPool.find(id) != objectPool.end()) {

        // Delete all objects in the pool
        for (unsigned int i = 0; i < objectPool[id].size(); i++) {

            objectPool[id][i]->isFromObjectPool = false; // Allows deletion
            objectPool[id][i]->destroy();
        }

        objectPool[id].clear();

        objectPool.erase(objectPool.find(id));
        objectPoolInUseMap.erase(objectPoolInUseMap.find(id));
    }
}

// Set every object in the pool, active or otherwise, to yeild
void GameObjectManager::yeildObjectPool(const unsigned int id, bool y) {
    
    if (objectPool.find(id) != objectPool.end()) {

        // Delete all objects in the pool
        for (unsigned int i = 0; i < objectPool[id].size(); i++) {

            objectPool[id][i]->setYeilding(y);
        }
    }
}
