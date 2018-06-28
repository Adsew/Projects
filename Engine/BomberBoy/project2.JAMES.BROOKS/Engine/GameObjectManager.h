#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

/*
Student: James Brooks
Class: Game Architecture

File: GameObjectManager.h

Class: GameObjectManager

Description: Maintains the memory and functionality of all game objects currently loaded.
*/


#include "ISystem.h"


class GameObject;


class GameObjectManager : public ISystem {

    friend class GameEngine;

    
    /***** Variables *****/

private:
	
    std::list<GameObject *> gameObjects;        // Currently updating
    std::list<GameObject *> recentlyCreated;    // To be added to list next cycle

    std::map<unsigned int, std::vector<GameObject *>> objectPool;
    std::map<unsigned int, std::vector<bool>> objectPoolInUseMap;

    unsigned int poolIDCount;


    /***** Functions *****/

private:

    GameObjectManager() = default;

    ~GameObjectManager();

    GameObjectManager(const GameObjectManager &gom) = default;

protected:

	virtual void initialize() override;
	
    virtual void clean() override;

    virtual void update() override;

public:

    inline static GameObjectManager &Instance() {

        static GameObjectManager instance;

        return instance;
    }

    // Create an empty game object
    GameObject *createGameObject();

    // Creates a game object using a prefab file, returns null if none found
    GameObject *createGameObjectFromPrefab(const char *prefab);

    // Find game object by given name
    GameObject *findGameObjectByName(const char *name);

        /* Object Pool Functions */

    // Creates a pool of the given game object, returns id to access pool
    // Does not take responsability for the sample game objects
    // Returns -1 on error
    unsigned int createObjectPool(GameObject *sample, const unsigned int amount);

    // Request a game object from the pool
    // NULL if none available or pool doesn't exist
    GameObject *requestFromPool(const unsigned int id);

    // Return an object to the pool for later use
    void returnToPool(const unsigned int id, GameObject *object);

    // Dispose of the object pool, deleting all objects in the pool
    void destroyObjectPool(const unsigned int id);

    // Set every object in the pool, active or otherwise, to yeild
    void yeildObjectPool(const unsigned int id, bool y);
};

#endif // !GAME_OBJECT_MANAGER_H
