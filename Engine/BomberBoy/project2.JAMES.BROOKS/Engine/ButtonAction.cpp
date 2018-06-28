/*
Student: James Brooks
Class: Game Architecture

File: ButtonAction.cpp

Class: ButtonAction

Description: Base class for a button action that will be set to a button to make it work
*/


#include "Core.h"

#include "UIButton.h"
#include "ButtonAction.h"


IMPLEMENT_ABSTRACT_COMPONENT(ButtonAction);


void ButtonAction::initialize() {

    functionSet = false;
}

void ButtonAction::update() {

    if (!functionSet) {

        if (UIButton *button = (UIButton *)gameObject->getComponent("UIButton")) {

            button->setOnClick(this);
        }
        functionSet = true;
    }
}

void ButtonAction::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {


}

Component &ButtonAction::operator=(const Component &comp) {

    return *this;
}
