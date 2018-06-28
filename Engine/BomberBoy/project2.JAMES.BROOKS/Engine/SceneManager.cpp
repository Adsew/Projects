/*
Student: James Brooks
Class: Game Architecture

File: SceneManager.cpp

Class: SceneManager

Description: Manages the active scene and allows for the loading and deloading of scenes.
*/


#include "Core.h"

#include "Scene.h"
#include "SceneManager.h"


SceneManager::~SceneManager() {

    unloadActiveScene();
}

void SceneManager::initialize() {

    availableScenes.clear();
    activeScene = NULL;
    newSceneName = "";
}

void SceneManager::clean() {

    unloadActiveScene();
}

void SceneManager::update() {

    if (newSceneName != "") {

        Scene *newScene = new Scene(Object::generateID(), newSceneName.c_str());

        if (loadScene(newScene)) {

            unloadActiveScene();

            activeScene = newScene;
        }
        else {

            DEBUG_LOG("SceneManager: Failed to load scene " << newSceneName << ".");
        }

        newSceneName = "";
    }
}

bool SceneManager::loadScene(Scene *scene) {

    return scene->load();
}

void SceneManager::unloadActiveScene() {

    if (activeScene != NULL) {

        delete activeScene;

        activeScene = NULL;
    }
}

// Unloads current scene and loads the one by given name
bool SceneManager::changeScene(const char *sceneName) {

    newSceneName = sceneName;

    for (std::list<std::string>::iterator iter = availableScenes.begin();
        iter != availableScenes.end();
        iter++) {

        if (*iter == newSceneName) {

            return true;
        }
    }

    newSceneName = "";

    return false;
}

// Add a scene name to the list of available scenes
void SceneManager::addScene(const char *sceneName) {

    availableScenes.push_back(sceneName);
}
