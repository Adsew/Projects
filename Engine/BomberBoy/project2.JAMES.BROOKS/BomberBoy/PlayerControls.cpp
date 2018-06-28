/*
Student: James Brooks
Class: Game Architecture

File: PlayerControls.cpp

Class: PlayerControls

Description: Component allowing input from the keyboard to move the player accordingly.
*/


#include "Core.h"

#include <SFML/Window/Keyboard.hpp>

#include "Timer.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Bomb.h"
#include "LevelData.h"
#include "Terrain.h"
#include "Transform.h"
#include "AnimatedSprite.h"
#include "PlayerControls.h"


#define DEFAULT_BOMB_INTERVAL 3
#define MOVE_COOLDOWN 0.2f


IMPLEMENT_COMPONENT(PlayerControls)


void PlayerControls::initialize() {

    deltaTimeInput = 0;
    deltaTimeBomb = 0;

    map = NULL;
    posX = 0;
    posY = 0;

    bombInterval = DEFAULT_BOMB_INTERVAL;
}

void PlayerControls::update() {

    InputManager &im = InputManager::Instance();
    Transform *trans = gameObject->getTransform();
    AnimatedSprite *anim = (AnimatedSprite *)gameObject->getComponent("AnimatedSprite");

    deltaTimeInput += Timer::Instance().getDelta();
    deltaTimeBomb += Timer::Instance().getDelta();

    if (deltaTimeInput >= MOVE_COOLDOWN) {

        deltaTimeInput = 0;

        // Check for exit first
        if (im.getKeyDown(sf::Keyboard::Escape)) {

            im.exit();
        }
        else if (trans != NULL && anim != NULL && map != NULL) {

            // movement
            if (im.getKeyDown(sf::Keyboard::A)) {

                if (map->requestMoveEntity(trans, posX, posY, -1, 0)) {

                    posX--;
                }
                
                anim->setAnimation(1);
            }
            if (im.getKeyDown(sf::Keyboard::D)) {

                if (map->requestMoveEntity(trans, posX, posY, 1, 0)) {

                    posX++;
                }

                anim->setAnimation(3);
            }
            if (im.getKeyDown(sf::Keyboard::W)) {

                if (map->requestMoveEntity(trans, posX, posY, 0, -1)) {

                    posY--;
                }

                anim->setAnimation(2);
            }
            if (im.getKeyDown(sf::Keyboard::S)) {

                if (map->requestMoveEntity(trans, posX, posY, 0, 1)) {

                    posY++;
                }

                anim->setAnimation(0);
            }

            // place bomb button
            if (im.getKeyDown(sf::Keyboard::Space)
                && deltaTimeBomb >= bombInterval) {

                deltaTimeBomb = 0;

                GameObject *bomb = GameObjectManager::Instance().requestFromPool(bombPoolID);

                if (bomb != NULL) {

                    Bomb *bombComp = (Bomb *)bomb->getComponent("Bomb");

                    if (bombComp != NULL) {

                        bombComp->playerOwned = true;
                        bombComp->resetBomb();
                        bombComp->placeAtMe(map, posX, posY);
                    }
                }
            }
        }
    }
}

void PlayerControls::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    FileSystem::Instance().getAttribute(accessor, "bombInterval", bombInterval);
    deltaTimeBomb = bombInterval;
}

Component &PlayerControls::operator=(const Component &comp) {

    return *this;
}

// Performs notifications and destroys player
void PlayerControls::kill() {

    level->decreaseLives();

    gameObject->destroy();
}

void PlayerControls::setLevel(LevelData *currentLevel) {

    level = currentLevel;
}

void PlayerControls::setMap(Terrain *m) {

    map = m;
}

void PlayerControls::setPosition(int x, int y) {

    posX = x;
    posY = y;
}

void PlayerControls::setBombPool(unsigned int id) {

    bombPoolID = id;
}
