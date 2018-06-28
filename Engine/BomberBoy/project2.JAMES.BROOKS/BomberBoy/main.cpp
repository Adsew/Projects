/*
Student: James Brooks
Class: Game Architecture

File: main.cpp

Description: Begin running the game engine.
*/


#include "Core.h"

#include "GameEngine.h"


void main() {

	GameEngine::Instance().initialize("res/Settings.config");
	GameEngine::Instance().gameLoop();
}