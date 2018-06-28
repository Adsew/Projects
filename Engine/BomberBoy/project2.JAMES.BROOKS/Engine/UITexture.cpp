/*
Student: James Brooks
Class: Game Architecture

File: UITexture.cpp

Class: UITexture

Description: Displays an image to the screen.
*/


#include "Core.h"

#include "TextureAsset.h"
#include "AssetManager.h"
#include "RenderSystem.h"
#include "UITexture.h"


IMPLEMENT_DERIVED_COMPONENT(UITexture, UIElement);


void UITexture::initialize() {

    UIElement::initialize();

    texture = NULL;
}

void UITexture::update() {


}

void UITexture::render() {

    if (gameObject != NULL) {
        if (gameObject->isActive()) {
            if (texture != NULL) {

                RenderSystem::Instance().getWindow()->draw(sprite);
            }
        }
    }
}

void UITexture::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    std::string assetName = "";
    int priority = UI_LOW_PRIORITY;
    float scaleX = 0, scaleY = 0;

    FileSystem::Instance().getAttribute(accessor, "width", rectangle.width);
    FileSystem::Instance().getAttribute(accessor, "height", rectangle.height);
    FileSystem::Instance().getAttribute(accessor, "xOrigin", rectangle.left);
    FileSystem::Instance().getAttribute(accessor, "yOrigin", rectangle.top);
    
    if (FileSystem::Instance().getAttribute(accessor, "xScale", scaleX)
        && FileSystem::Instance().getAttribute(accessor, "yScale", scaleY)) {

        sprite.setScale(sf::Vector2f((float)scaleX, (float)scaleY));
    }

    if (FileSystem::Instance().getAttribute(accessor, "priority", priority)) {

        setRenderPriority(priority);
    }

    if (FileSystem::Instance().getAttribute(accessor, "asset", assetName)) {

        TextureAsset *temp = NULL;

        // Maintain asset reference count and make texture asset remains usable
        texture = AssetManager::Instance().getAsset(assetName.c_str());

        temp = (TextureAsset *)texture.get();

        if (temp != NULL) {

            sprite.setTexture(*(temp->getTexture()));
            sprite.setTextureRect(rectangle);
        }
        else {

            DEBUG_LOG("UITexture: Unable to retrieve texture asset for " << assetName.c_str() << ".");
        }
    }
}

Component &UITexture::operator=(const Component &comp) {

    UITexture *orig = (UITexture *)&comp;

    texture = orig->texture;
    rectangle = orig->rectangle;

    sprite.setTexture(*((TextureAsset *)texture.get())->getTexture());
    sprite.setTextureRect(rectangle);

    return *this;
}

void UITexture::setTexture(const char *name) {

    texture = AssetManager::Instance().getAsset(name);
}
