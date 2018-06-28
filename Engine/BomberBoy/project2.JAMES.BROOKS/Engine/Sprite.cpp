/*
Student: James Brooks
Class: Game Architecture

File: Sprite.cpp

Class: Sprite

Description: A small image to be rendered to the screen.
*/


#include "Core.h"

#include "Transform.h"
#include "RenderSystem.h"
#include "AssetManager.h"
#include "TextureAsset.h"
#include "Sprite.h"


IMPLEMENT_COMPONENT(Sprite);


void Sprite::initialize() {

    setRenderPriority(LOWEST_PRIORITY);
}

void Sprite::update() {
    
    Transform *trans = gameObject->getTransform();
    
    if (trans != NULL) {

        sprite.setPosition(trans->position);
        sprite.setRotation(trans->rotation);
        sprite.setScale(trans->scale);
    }
}

void Sprite::render() {
    
    if (gameObject != NULL) {
        if (gameObject->isActive()) {

            RenderSystem::Instance().getWindow()->draw(sprite);
        }
    }
}

void Sprite::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    std::string assetName = "";
    int priority = LOWEST_PRIORITY;

    FileSystem::Instance().getAttribute(accessor, "width", spriteRect.width);
    FileSystem::Instance().getAttribute(accessor, "height", spriteRect.height);
    FileSystem::Instance().getAttribute(accessor, "xOrigin", spriteRect.left);
    FileSystem::Instance().getAttribute(accessor, "yOrigin", spriteRect.top);

    if (FileSystem::Instance().getAttribute(accessor, "priority", priority)) {

        setRenderPriority(priority);
    }

    if (FileSystem::Instance().getAttribute(accessor, "Asset", assetName)) {

        TextureAsset *temp = NULL;

        // Maintain asset reference count and make texture asset remains usable
        spriteImagePtr = AssetManager::Instance().getAsset(assetName.c_str());

        temp = (TextureAsset *)spriteImagePtr.get();

        if (temp != NULL) {

            sprite.setTexture(*(temp->getTexture()));
            sprite.setTextureRect(spriteRect);
        }
        else {

            DEBUG_LOG("Sprite: Unable to retrieve texture asset for " << assetName.c_str() << ".");
        }
    }
}

Component &Sprite::operator=(const Component &comp) {

    const Sprite *orig = (const Sprite *)&comp;
    TextureAsset *tempTex = NULL;

    spriteImagePtr = orig->spriteImagePtr;
    spriteRect.width = orig->spriteRect.width;
    spriteRect.height = orig->spriteRect.height;
    spriteRect.top = orig->spriteRect.top;
    spriteRect.left = orig->spriteRect.left;

    tempTex = (TextureAsset *)spriteImagePtr.get();

    if (tempTex != NULL) {

        sprite.setTexture(*(tempTex->getTexture()));
        sprite.setTextureRect(spriteRect);
    }

    setRenderPriority(orig->getRenderPriority());

    return *this;
}

void Sprite::setRectangle(int width, int height, int x, int y) {

    spriteRect.width = width;
    spriteRect.height = height;
    spriteRect.left = x;
    spriteRect.top = y;

    sprite.setTextureRect(spriteRect);
}

void Sprite::setTextureAsset(const char *assetName) {

    TextureAsset *temp = NULL;

    std::shared_ptr<Asset> tempAsset = AssetManager::Instance().getAsset(assetName);

    temp = (TextureAsset *)tempAsset.get();

    if (temp != NULL) {

        spriteImagePtr = tempAsset;

        sprite.setTexture(*(temp->getTexture()));
    }
    else {

        DEBUG_LOG("Sprite: Unable to retrieve texture asset for " << assetName << ".");
    }
}
