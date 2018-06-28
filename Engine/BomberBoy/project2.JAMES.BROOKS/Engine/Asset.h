#ifndef ASSET_H
#define ASSET_H

/*
Student: James Brooks
Class: Game Architecture

File: Asset.h

Class: Asset

Description: A base class for all future assets of any time to be created.
*/


#include "AssetManager.h"
#include "Object.h"


class Asset : public Object {

    friend class AssetManager;

    /***** Variables *****/

private:

    std::string name;


    /***** Functions *****/

protected:

	Asset(unsigned int uniqueID);

    void initialize() override {}

    void onDestroy() override {}

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &element) = 0;

public:

    virtual ~Asset();

    void setName(const char *n);
};

#endif // !ASSET_H
