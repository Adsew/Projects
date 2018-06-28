/*
Student: James Brooks
Class: Game Architecture

File: GameEngine.cpp

Class: GameEngine

Description: Holds the game loop and runs the major systems that run the game.
*/


#include "Core.h"

#include "AssetManager.h"
#include "FileSystem.h"
#include "InputManager.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"
#include "GameEngine.h"


// Initialize all systems needed to run the game. Call before game loop
void GameEngine::initialize(const char *settingsFile) {

    InputManager::Instance().initialize();
    Timer::Instance().initialize();
    AssetManager::Instance().initialize();
	RenderSystem::Instance().initialize();
    SceneManager::Instance().initialize();
	GameObjectManager::Instance().initialize();
    ComponentManager::Instance().initialize();
    
	// Needs to be at the end because we will load a default file
	FileSystem::Instance().initialize(settingsFile);
}

// Called automatically at the end of gameLoop to clean system memory
void GameEngine::clean() {

    SceneManager::Instance().clean();
    GameObjectManager::Instance().clean();
    ComponentManager::Instance().clean();
    RenderSystem::Instance().clean();
    AssetManager::Instance().clean();

    Timer::Instance().clean();
    InputManager::Instance().clean();
}

// Main game loop that runs the game
void GameEngine::gameLoop() {

    Timer::Instance().beginTiming();

	while (!InputManager::Instance().gameCloseTriggered) {

		InputManager::Instance().update();

		AssetManager::Instance().update();

		GameObjectManager::Instance().update();

        ComponentManager::Instance().update();

		RenderSystem::Instance().update();

        SceneManager::Instance().update();

        Timer::Instance().update();

		//DEBUG_LOG("Current Game Time: " << Timer::Instance().runTime << " Delta Time: " << Timer::Instance().deltaTime);
	}

    clean();
}
