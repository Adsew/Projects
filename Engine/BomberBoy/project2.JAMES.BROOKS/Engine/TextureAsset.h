#ifndef TEXTURE_ASSET_H
#define TEXTURE_ASSET_H


/*
Student: James Brooks
Class: Game Architecture

File: TextureAsset.h

Class: TextureAsset

Description: An asset that holds an image to be used as a texture.
*/


#include "Asset.h"


namespace sf {

    class Texture;
}


class TextureAsset : public Asset {

    REGISTER_ASSET(TextureAsset);


    /***** Variables *****/

private:

    sf::Texture *texture;

    std::string fileName;


    /***** Functions *****/

public:

    TextureAsset(unsigned int uniqueID);
    
    virtual ~TextureAsset();

    void load(std::unique_ptr<FileSystem::FileAccessor> &element) override;

    sf::Texture *getTexture();
};

#endif // !TEXTURE_ASSET_H
