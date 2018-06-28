#ifndef SPRITE_H
#define SPRITE_H

/*
Student: James Brooks
Class: Game Architecture

File: Sprite.h

Class: Sprite

Description: A small image to be rendered to the screen.
*/


#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Component.h"
#include "IRenderable.h"


class Asset;
class TextureAsset;


class Sprite : public Component, public IRenderable {

    REGISTER_COMPONENT(Sprite);


    /***** Variables *****/

protected:

    std::shared_ptr<Asset> spriteImagePtr;
    sf::Sprite sprite;
    sf::IntRect spriteRect;
    

    /***** Functions *****/

protected:

    virtual void initialize() override;

    virtual void update() override;

    virtual void render() override;

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

public:

    virtual Component &operator=(const Component &comp) override;

    /* Gets/Sets */

    void setRectangle(int width, int height, int x, int y);

    void setTextureAsset(const char *name);
};

#endif // !SPRITE_H
