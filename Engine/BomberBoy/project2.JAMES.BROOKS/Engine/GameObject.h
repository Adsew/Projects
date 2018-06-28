#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

/*
Student: James Brooks
Class: Game Architecture

File: GameObject.h

Class: GameObject

Description: A standard object to be created into the scene.
*/


#include "Object.h"


class Scene;
class Transform;
class Component;


class GameObject : public Object {

    friend class GameObjectManager;

    friend class Scene;

    /***** Variables *****/

private:
	
    Scene *scene;

    std::string name;

    Transform *transform;

    std::map<std::string, Component*> components;

    unsigned int objectPoolID;

    bool isFromObjectPool;

    bool active;
    bool yeilding;


    /***** Functions *****/

protected:
	
    GameObject(unsigned int uniqueID);

    GameObject(unsigned int uniqueID, const char *goName);

    GameObject(unsigned int uniqueID, const std::string &goName);

    virtual ~GameObject();

    // Initialize GO to a usable state
    void initialize() override;

    // Destroy data contained before deletion
    void onDestroy() override;

    // Cycle update updates all game object components
    virtual void update();

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor);

public:

    // Add a component to the game object for use
	void addComponent(Component* component);
	
    // Remove the component from the game object, if exists
    void removeComponent(Component* component);

    // Create a clone of this gameobject with the same components and settings
    GameObject *clone();

    // Enable or disable updating of this game object and all its components
    void setActive(bool a);

    // Check if gameobject is enabled
    bool isActive();

    // Sets the game object to no longer update but still render
    void setYeilding(bool y);

    // Check if game object is yeilding
    bool isYeilding();

        /* Gets/Sets */

    void setName(const char *newName);

    void setName(const std::string &newName);

    std::string &getName();

    // Uses scene of the given game object
    void setScene(GameObject *go);

    Component *getComponent(const std::string &type);

    Transform *getTransform();
};

#endif // !GAME_OBJECT_H
