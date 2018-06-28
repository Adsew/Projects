/*
Student: James Brooks
Class: Game Architecture

File: ButtonActionSceneChange.cpp

Class: ButtonActionSceneChange

Description: Button action that changes scenes when clicked
*/


#include "Core.h"

#include "SceneManager.h"
#include "UIButton.h"
#include "ButtonActionSceneChange.h"


IMPLEMENT_DERIVED_COMPONENT(ButtonActionSceneChange, ButtonAction);


void ButtonActionSceneChange::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    FileSystem::Instance().getAttribute(accessor, "scene", nextScene);
}

Component &ButtonActionSceneChange::operator=(const Component &comp) {

    ButtonActionSceneChange *orig = (ButtonActionSceneChange *)&comp;

    nextScene = orig->nextScene;

    return *this;
}

void ButtonActionSceneChange::setScene(const char *scene) {

    nextScene = scene;
}

void ButtonActionSceneChange::onClick() {

    SceneManager::Instance().changeScene(nextScene.c_str());
}
