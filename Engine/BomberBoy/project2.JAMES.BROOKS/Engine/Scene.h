#ifndef SCENE_H
#define SCENE_H

/*
Student: James Brooks
Class: Game Architecture

File: Scene.h

Class: Scene

Description: Holds multiple game objects grouped together to make a scene.
    Ensures easier management of game objects active at a time.
*/


#include "Object.h"


class GameObject;


class Scene : public Object {

    friend class SceneManager;


    /***** Variables *****/

    std::list<GameObject *> gameObjects;

    std::string name;


    /***** Functions *****/

protected:

    Scene(unsigned int id, const char *n);

    virtual ~Scene();

    void initialize() override;

    void onDestroy() override;

    // Loads a scene file with the same name as the scene
    bool load();

public:
    
    // Make the game object part of this scene
    void manageGameObject(GameObject *go);

    // Remove game object from the list of those managed by this scene (does not delete GO)
    void unmanageGameObject(GameObject *go);
};

#endif // !SCENE_H
