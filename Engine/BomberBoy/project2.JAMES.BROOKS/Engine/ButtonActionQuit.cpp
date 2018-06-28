/*
Student: James Brooks
Class: Game Architecture

File: ButtonActionQuit.cpp

Class: ButtonActionQuit

Description: Button action that quits the game on click
*/


#include "Core.h"

#include "InputManager.h"
#include "ButtonActionQuit.h"


IMPLEMENT_DERIVED_COMPONENT(ButtonActionQuit, ButtonAction);


void ButtonActionQuit::onClick() {

    InputManager::Instance().exit();
}
