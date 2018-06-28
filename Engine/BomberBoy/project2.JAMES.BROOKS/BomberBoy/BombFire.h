#ifndef BOMB_FIRE_H
#define BOMB_FIRE_H

/*
Student: James Brooks
Class: Game Architecture

File: BombFire.h

Class: BombFire

Description: Spawned by a bomb after it detonates and kills enemies and players it touches.
*/


#include "Component.h"


class Terrain;


class BombFire : public Component {

    REGISTER_COMPONENT(BombFire);


    /***** Variables *****/

    Terrain *map;

    bool playerOwned;

    int posX, posY;

    float deathTimer;
    float timeTilDeath;


    /***** Functions *****/

protected:

    virtual void initialize() override;

    virtual void update() override;

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

public:

    virtual Component &operator=(const Component &comp) override;

    // Set the map the fire is on
    void setMap(Terrain *t, int x, int y);

    // Set the owner of who spawned the bomb
    void setPlayerOwned(bool b);
};

#endif // !BOMB_FIRE_H
