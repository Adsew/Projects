#include "Core.h"
#include "Engine.h"
#include <ctime>

#include "InputManager.h"
#include "FileManager.h"
#include "AssetManager.h"
#include "GameObjectManager.h"
#include "RenderManager.h"

Engine::Engine()
	: m_running(false)
{
}

Engine::~Engine()
{
	glfwTerminate();
}

void Engine::gameLoop()
{
	float gameTime = 0.0f;
	clock_t _time;
	float deltaTime = 0.0f;

	while (m_running)
	{
		_time = clock();

		InputManager::instance().update(deltaTime);
		FileManager::instance().update(deltaTime);
		AssetManager::instance().update(deltaTime);
		GameObjectManager::instance().update(deltaTime);
		RenderManager::instance().update(deltaTime);
		_time = clock() - _time;
		deltaTime = ((float)_time) / ((clock_t)1000);
		gameTime += deltaTime;
		//std::cout << "Current Game Time: " << gameTime << " Delta Time: " << deltaTime << std::endl;
	}
}

void Engine::initialize()
{
	if (!glfwInit())
	{
		std::cout << "GLFW initialization failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::srand(unsigned(std::time(nullptr)));

	m_running = true;
	RenderManager::instance().initialize();
	AssetManager::instance().initialize();
	InputManager::instance().initialize();

	GameObjectManager::instance().initialize();

	FileManager::instance().initialize();
}

void Engine::requestShutdown()
{
	m_running = false;
}
