/*
Student: James Brooks
Class: Game Architecture

File: EnemyAI.cpp

Class: EnemyAI

Description: Controls the actions the enemies will take.
    They will move randomly and drop bombs.
*/


#include "Core.h"

#include "GameObjectManager.h"
#include "Transform.h"
#include "LevelData.h"
#include "Bomb.h"
#include "Terrain.h"
#include "AnimatedSprite.h"
#include "EnemyAI.h"


IMPLEMENT_COMPONENT(EnemyAI);


void EnemyAI::initialize() {

    posX = 0;
    posY = 0;

    bombPlaceInterval = 0;
    bombPlaceChance = 0;
    moveInterval = 0;

    deltaTimeMove = 0;
    deltaTimeBomb = 0;
}

void EnemyAI::update() {

    if (map != NULL) {
        
        deltaTimeMove += Timer::Instance().getDelta();
        deltaTimeBomb += Timer::Instance().getDelta();

        // Movement update
        if (deltaTimeMove >= moveInterval) {

            AnimatedSprite *anim = (AnimatedSprite *)gameObject->getComponent("AnimatedSprite");
            int direction = rand() % 4;

            deltaTimeMove -= moveInterval;

            // Right
            if (direction == 0) {
                if (map->requestMoveEntity(gameObject->getTransform(), posX, posY, 1, 0)) {

                    posX++;
                    anim->setAnimation(3);
                }
            }
            // Left
            else if (direction == 1) {
                if (map->requestMoveEntity(gameObject->getTransform(), posX, posY, -1, 0)) {

                    posX--;
                    anim->setAnimation(1);
                }
            }
            // Up
            else if (direction == 2) {
                if (map->requestMoveEntity(gameObject->getTransform(), posX, posY, 0, -1)) {

                    posY--;
                    anim->setAnimation(2);
                }
            }
            // Down
            else if (direction == 3) {
                if (map->requestMoveEntity(gameObject->getTransform(), posX, posY, 0, 1)) {

                    posY++;
                    anim->setAnimation(0);
                }
            }
        }

        // Bomb placing update
        if (deltaTimeBomb >= bombPlaceInterval) {

            deltaTimeBomb -= bombPlaceInterval;

            // Chance to place bomb so they dont place on an exact timing
            if ((rand() % 100) < bombPlaceChance) {

                GameObject *bomb = GameObjectManager::Instance().requestFromPool(bombPoolID);

                if (bomb != NULL) {

                    Bomb *bombComp = (Bomb *)bomb->getComponent("Bomb");

                    if (bombComp != NULL) {

                        bombComp->playerOwned = false;
                        bombComp->resetBomb();
                        bombComp->placeAtMe(map, posX, posY);
                    }
                }
            }
        }
    }
}

void EnemyAI::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    FileSystem::Instance().getAttribute(accessor, "bombInterval", bombPlaceInterval);
    FileSystem::Instance().getAttribute(accessor, "bombChance", bombPlaceChance);
    FileSystem::Instance().getAttribute(accessor, "moveInterval", moveInterval);
}

Component &EnemyAI::operator=(const Component &comp) {

    const EnemyAI *orig = (const EnemyAI *)&comp;

    bombPlaceInterval = orig->bombPlaceInterval;
    bombPlaceChance = orig->bombPlaceChance;
    moveInterval = orig->moveInterval;

    return *this;
}

// Kill the enemy and increase the score
void EnemyAI::kill(int score) {

    if (level != NULL) {

        level->removeEntity(gameObject);
        level->increaseScore(score);
        gameObject->destroy();
    }
}

void EnemyAI::setLevel(LevelData *currentLevel) {

    level = currentLevel;
}

void EnemyAI::setMap(Terrain *m) {

    map = m;
}

void EnemyAI::setPosition(int x, int y) {

    posX = x;
    posY = y;
}

void EnemyAI::setBombPool(unsigned int id) {

    bombPoolID = id;
}
