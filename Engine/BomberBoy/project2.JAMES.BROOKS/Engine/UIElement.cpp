/*
Student: James Brooks
Class: Game Architecture

File: UIElement.cpp

Class: UIElement

Description: An element that will be rendered to the screen as visual UI.
Other classes will derive from this to create specific ui elements.
*/


#include "Core.h"

#include "UIElement.h"


IMPLEMENT_ABSTRACT_COMPONENT(UIElement);


void UIElement::initialize() {

    setRenderPriority(UI_LOW_PRIORITY);
}

void UIElement::setRectangle(int x, int y, int width, int height) {

    rectangle.top = y;
    rectangle.left = x;
    rectangle.width = width;
    rectangle.height = height;
}
