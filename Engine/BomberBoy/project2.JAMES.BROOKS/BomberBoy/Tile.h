#ifndef TILE_H
#define TILE_H

/*
Student: James Brooks
Class: Game Architecture

File: Tile.h

Class: Tile

Description: Component that defines the type of tile in the terrain grid.
    Types are: Collidable, Not Collidable
*/


#include "Component.h"


class Terrain;


class Tile : public Component {

    REGISTER_COMPONENT(Tile);

    friend class Terrain;


    /***** Variables *****/

protected:

    Terrain *map;
    int posX, posY;
    bool collidable;
    bool destroyable;


    /***** Functions *****/

protected:
    
    virtual void initialize() override;

    virtual void update() override;

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

public:

    virtual Component &operator=(const Component &comp) override;

    void setMapPosition(int x, int y, Terrain *map);

    void setCollidable(bool c);

    bool isCollidable();

    void setDestructable(bool d);

    bool isDestructable();
};

#endif // !TILE_H
