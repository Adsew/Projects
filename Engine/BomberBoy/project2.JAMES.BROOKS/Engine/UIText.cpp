/*
Student: James Brooks
Class: Game Architecture

File: UIText.cpp

Class: UIText

Description: Displays text to the screen.
*/


#include "Core.h"

#include "RenderSystem.h"
#include "UIText.h"


IMPLEMENT_DERIVED_COMPONENT(UIText, UIElement);


void UIText::initialize() {

    UIElement::initialize();

    displayFont.loadFromFile("res/Yes_Union.ttf");
    displayText.setFont(displayFont);
}

void UIText::update() {


}

void UIText::render() {

    if (gameObject != NULL) {
        if (gameObject->isActive()) {

            RenderSystem::Instance().getWindow()->draw(displayText);
        }
    }
}

void UIText::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    std::string loadedText = "";
    int priority = UI_LOW_PRIORITY;
    int size = 15;
    int x = 0, y = 0;

    if (FileSystem::Instance().getAttribute(accessor, "priority", priority)) {

        setRenderPriority(priority);
    }

    if (FileSystem::Instance().getAttribute(accessor, "text", loadedText)) {

        setText(loadedText.c_str());
    }
    if (FileSystem::Instance().getAttribute(accessor, "size", size)) {

        displayText.setCharacterSize(size);

    }
    if (FileSystem::Instance().getAttribute(accessor, "position.x", x) && FileSystem::Instance().getAttribute(accessor, "position.y", y)) {

        setPosition(x, y);
    }
    if (FileSystem::Instance().traverseToElement(accessor, "colour")) {

        int r = 255, g = 255, b = 255, a = 255;

        FileSystem::Instance().getAttribute(accessor, "r", r);
        FileSystem::Instance().getAttribute(accessor, "g", g);
        FileSystem::Instance().getAttribute(accessor, "b", b);
        FileSystem::Instance().getAttribute(accessor, "a", a);

        displayText.setFillColor(sf::Color(r, g, b, a));

        FileSystem::Instance().traverseToParentElement(accessor);
    }
}

Component &UIText::operator=(const Component &comp) {

    UIText *orig = (UIText *)&comp;

    displayFont = orig->displayFont;
    displayText.setFont(displayFont);

    displayText.setString(orig->displayText.getString());
    displayText.setCharacterSize(orig->displayText.getCharacterSize());
    displayText.setFillColor(orig->displayText.getFillColor());
    displayText.setPosition(orig->displayText.getPosition());

    return *this;
}

void UIText::setText(const char *text) {

    mytext = text;
    displayText.setString(mytext);
}

void UIText::setSize(int x, int y) {

    displayText.setScale((float)x, (float)y);
}

void UIText::setColour(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a) {

    displayText.setFillColor(sf::Color(r, g, b, a));
}

void UIText::setPosition(int x, int y) {

    displayText.setPosition((float)x, (float)y);
}
