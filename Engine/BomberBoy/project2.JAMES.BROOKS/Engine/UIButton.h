#ifndef UI_BUTTON_H
#define UI_BUTTON_H

/*
Student: James Brooks
Class: Game Architecture

File: UIButton.h

Class: UIButton

Description: Displays a button with text to the screen that calls a set
    function when the rectangle is clicked.
*/


#include "UIElement.h"


class ButtonAction;


class UIButton : public UIElement {

    REGISTER_COMPONENT(UIButton);

    /***** Variables *****/

protected:

    std::shared_ptr<Asset> texture;

    sf::Text displayText;
    sf::Font displayFont;
    sf::Sprite sprite;
    sf::Vector2i buttonPosition;
    sf::Vector2f buttonScale;

    ButtonAction *clicker;

    float deltaTime;

    
    /***** Functions *****/

protected:
    
    virtual void initialize() override;

    virtual void update() override;

    virtual void render() override;

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

public:

    virtual Component &operator=(const Component &comp) override;

    void setOnClick(ButtonAction *clickAction);

    void setText(const char *text);

    void setSize(int x, int y);

    void setColour(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);

    void setTextPosition(int x, int y);

    void setButtonPosition(int x, int y);

    void setTexture(const char *name);

    void setTextureScale(int x, int y);
};

#endif // !UI_BUTTON_H
