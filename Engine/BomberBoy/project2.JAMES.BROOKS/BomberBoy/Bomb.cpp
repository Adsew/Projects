/*
Student: James Brooks
Class: Game Architecture

File: Bomb.cpp

Class: Bomb

Description: Represents an object that will detonate after a set time.
*/


#include "Core.h"

#include "GameObjectManager.h"
#include "AnimatedSprite.h"
#include "Tile.h"
#include "Terrain.h"
#include "BombFire.h"
#include "Bomb.h"


IMPLEMENT_COMPONENT(Bomb);


void Bomb::initialize() {

    map = NULL;

    posX = 0;
    posY = 0;

    exploded = false;
    playerOwned = false;
}

void Bomb::update() {

    if (anim != NULL) {
        if (anim->hasPlayedCountTimes()) {

            explode();
        }
    }
}

void Bomb::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {


}

Component &Bomb::operator=(const Component &comp) {

    const Bomb *orig = (const Bomb *)&comp;

    posX = orig->posX;
    posY = orig->posY;

    firePoolID = orig->firePoolID;

    return *this;
}

// Reset the bomb to a pre-exploded state
void Bomb::resetBomb() {

    anim = (AnimatedSprite *)gameObject->getComponent("AnimatedSprite");

    if (anim != NULL) {

        anim->resetAnimation();
    }

    exploded = false;
}

// Place the bomb at the given location in the map
void Bomb::placeAtMe(Terrain *terrain, int x, int y) {

    if (terrain != NULL) {

        map = terrain;

        map->placeEntityOnMapNoCollision(gameObject, x, y);

        posX = x;
        posY = y;
    }
}

// Called when the bomb explodes after the animation finishes
void Bomb::explode() {

    exploded = true;

    if (map != NULL) {
        
        // Create fire sprites from the bombs position
        for (int i = 0; i < 5; i++) {

            int x = 0, y = 0;
            bool spawnFire = false;

            if (i == 0) {

                x = posX;
                y = posY;
            }
            // Up fire
            else if (i == 1) {

                x = posX;
                y = posY - 1;
            }
            // Down fire
            else if (i == 2) {

                x = posX;
                y = posY + 1;
            }
            // Left fire
            else if (i == 3) {

                x = posX - 1;
                y = posY;
            }
            // Right fire
            else if (i ==4 ) {

                x = posX + 1;
                y = posY;
            }

            // Check that the fire can be placed here
            if (GameObject *go = map->checkCollisionOnMap(x, y)) {
                if (Tile *tile = (Tile *)go->getComponent("Tile")) {
                    if (tile->isDestructable()) {

                        spawnFire = true;
                    }
                }
                else {

                    spawnFire = true;
                }
            }
            else {
                
                spawnFire = true;
            }

            if (spawnFire) {

                if (GameObject *fire = GameObjectManager::Instance().requestFromPool(firePoolID)) {
                    if (BombFire *fireScript = (BombFire *)fire->getComponent("BombFire")) {

                        fireScript->setPlayerOwned(playerOwned);
                        fireScript->setMap(map, x, y);

                        map->placeEntityOnMapNoCollision(fire, x, y);
                    }
                }
            }
        }
    }

    gameObject->destroy();
}

// Set the fire pool ID to be used when exploding
void Bomb::setFirePoolID(unsigned int fID) {

    firePoolID = fID;
}

// FOR SUPER BOMB POWERUP
//for (int i = 0; i < 9; i++) {
//
//    int x = 0, y = 0;
//    bool spawnFire = false;
//
//    if (i == 0) {
//
//        x = posX;
//        y = posY;
//    }
//    // Up fire
//    else if (i > 0 && i < 3) {
//
//        x = posX;
//        y = posY - i;
//    }
//    // Down fire
//    else if (i > 2 && i < 5) {
//
//        x = posX;
//        y = posY + i - 2;
//    }
//    // Left fire
//    else if (i > 4 && i < 7) {
//
//        x = posX - i + 4;
//        y = posY;
//    }
//    // Right fire
//    else if (i > 6 && i < 9) {
//
//        x = posX + i - 6;
//        y = posY;
//    }