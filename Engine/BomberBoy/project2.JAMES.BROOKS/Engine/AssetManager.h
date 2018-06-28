#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

/*
Student: James Brooks
Class: Game Architecture

File: AssetManager.h

Class: AssetManager

Description: Manages all assets created during the course of the game. Responsible for
    deleting and creating assets.
*/


#include "ISystem.h"


class Asset;


class AssetManager : public ISystem {

    friend class GameEngine;


    /***** Variables *****/

private:

	std::map<std::string, std::shared_ptr<Asset>> assets;

    std::map<std::string, std::function<Asset *(unsigned int)>> assetCreate;


    /***** Functions *****/

private:

    AssetManager() = default;

    ~AssetManager() = default;

    AssetManager(const AssetManager& other) = default;

protected:

	virtual void initialize() override;

    virtual void clean() override;

    virtual void update() override;

    bool loadAsset(const char *assetName);

public:

    inline static AssetManager &Instance() {

        static AssetManager instance;

        return instance;
    }

    void addAssetType(const char *name, std::function<Asset *(unsigned int)> creationFunc);

    std::shared_ptr<Asset> getAsset(const char *assetName);
};

#endif // !ASSET_MANAGER_H
