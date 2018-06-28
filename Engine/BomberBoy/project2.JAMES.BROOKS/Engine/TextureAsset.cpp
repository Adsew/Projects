/*
Student: James Brooks
Class: Game Architecture

File: TextureAsset.cpp

Class: TextureAsset

Description: An asset that holds an image to be used as a texture.
*/


#include "Core.h"

#include <SFML/Graphics.hpp>

#include "TextureAsset.h"


IMPLEMENT_ASSET(TextureAsset);


TextureAsset::TextureAsset(unsigned int uniqueID) : Asset(uniqueID) {

    texture = NULL;
    fileName = "";
}


TextureAsset::~TextureAsset() {

    if (texture != NULL) {

        delete texture;
        texture = NULL;
    }
}

void TextureAsset::load(std::unique_ptr<FileSystem::FileAccessor> &element) {

    FileSystem::Instance().getAttribute(element, "path", fileName);

    texture = new sf::Texture();

    if (!texture->loadFromFile(fileName)) {

        DEBUG_LOG("TextureAsset: Failed to load file at " << fileName.c_str() << ".");

        delete texture;
        texture = NULL;
    }
}

sf::Texture *TextureAsset::getTexture() {

    return texture;
}
