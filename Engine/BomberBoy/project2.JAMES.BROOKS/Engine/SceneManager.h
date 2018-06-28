#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

/*
Student: James Brooks
Class: Game Architecture

File: SceneManager.h

Class: SceneManager

Description: Manages the active scene and allows for the loading and deloading of scenes.
*/


#include "ISystem.h"


class Scene;


class SceneManager : public ISystem {

    friend class GameEngine;


    /***** Variables *****/

private:

    std::list<std::string> availableScenes;

    Scene *activeScene;

    std::string newSceneName;


    /***** Functions *****/

private:

    SceneManager() = default;

    SceneManager(const SceneManager &sm) = default;

    virtual ~SceneManager();

protected:

    virtual void initialize() override;

    virtual void clean() override;

    virtual void update() override;

    bool loadScene(Scene *scene);

    void unloadActiveScene();

public:

    inline static SceneManager &Instance() {

        static SceneManager instance;

        return instance;
    }

    // Unloads current scene and loads the one by given name
    bool changeScene(const char *sceneName);

    // Add a scene name to the list of available scenes
    void addScene(const char *sceneName);
};


#endif // !SCENE_MANAGER_H
