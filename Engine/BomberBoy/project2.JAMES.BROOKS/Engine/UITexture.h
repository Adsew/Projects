#ifndef UI_TEXTURE_H
#define UI_TEXTURE_H

/*
Student: James Brooks
Class: Game Architecture

File: UITexture.h

Class: UITexture

Description: Displays an image to the screen.
*/


#include "UIElement.h"


class UITexture : public UIElement {

    REGISTER_COMPONENT(UITexture);

    /***** Variables *****/

protected:

    std::shared_ptr<Asset> texture;

    sf::Sprite sprite;


    /***** Functions *****/

protected:

    virtual void initialize() override;

    virtual void update() override;

    virtual void render() override;

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

public:

    virtual Component &operator=(const Component &comp) override;

    void setTexture(const char *name);
};

#endif // !UI_TEXTURE_H
