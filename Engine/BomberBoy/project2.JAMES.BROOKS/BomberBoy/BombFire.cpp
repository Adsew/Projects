/*
Student: James Brooks
Class: Game Architecture

File: BombFire.cpp

Class: BombFire

Description: Spawned by a bomb after it detonates and kills enemies and players it touches.
*/


#include "Core.h"

#include "Timer.h"
#include "Terrain.h"
#include "Tile.h"
#include "EnemyAI.h"
#include "PlayerControls.h"
#include "LevelData.h"
#include "BombFire.h"


IMPLEMENT_COMPONENT(BombFire);


void BombFire::initialize() {

    map = NULL;

    posX = 0;
    posY = 0;

    timeTilDeath = 0;
    deathTimer = 0;

    playerOwned = false;
}

void BombFire::update() {

    deathTimer += Timer::Instance().getDelta();

    if (map != NULL) {

        GameObject *goHit = map->checkCollisionOnMap(posX, posY);

        if (goHit != NULL) {

            if (Tile *tile = (Tile *)goHit->getComponent("Tile")) {

                if (tile->isDestructable()) {

                    map->removeAndDestroyObject(posX, posY);
                }
            }
            if (EnemyAI *enemy = (EnemyAI *)goHit->getComponent("EnemyAI")) {

                if (playerOwned) {

                    enemy->kill(1);
                }
                else {

                    enemy->kill(0);
                }

                map->removeAndDestroyObject(posX, posY);
            }
            if (PlayerControls *player = (PlayerControls *)goHit->getComponent("PlayerControls")) {

                map->removeAndDestroyObject(posX, posY);
                player->kill();
            }
        }
    }

    if (deathTimer >= timeTilDeath) {

        deathTimer = 0;

        gameObject->destroy();
    }
}

void BombFire::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    FileSystem::Instance().getAttribute(accessor, "burnout", timeTilDeath);
}

Component &BombFire::operator=(const Component &comp) {

    const BombFire *orig = (const BombFire *)&comp;

    timeTilDeath = orig->timeTilDeath;

    return *this;
}

// Set the map the fire is on
void BombFire::setMap(Terrain *t, int x, int y) {

    map = t;

    posX = x;
    posY = y;
}

// Set the owner of who spawned the bomb
void BombFire::setPlayerOwned(bool b) {

    playerOwned = b;
}
