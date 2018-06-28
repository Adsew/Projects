#ifndef UI_TEXT_H
#define UI_TEXT_H

/*
Student: James Brooks
Class: Game Architecture

File: UIText.h

Class: UIText

Description: Displays text to the screen.
*/


#include "UIElement.h"


class UIText : public UIElement {

    REGISTER_COMPONENT(UIText);

    /***** Variables *****/

protected:

    sf::Text displayText;
    sf::Font displayFont;
    std::string mytext;


    /***** Functions *****/

protected:
    
    virtual void initialize() override;

    virtual void update() override;

    virtual void render() override;

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

public:

    virtual Component &operator=(const Component &comp) override;

    void setText(const char *text);
    
    void setSize(int x, int y);

    void setColour(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);

    void setPosition(int x, int y);
};

#endif // !IU_TEXT_H
