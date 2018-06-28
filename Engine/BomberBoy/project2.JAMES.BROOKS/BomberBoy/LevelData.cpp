/*
Student: James Brooks
Class: Game Architecture

File: LevelData.cpp

Class: LevelData

Description: Keeps track of the entities within the current level and the score.
*/


#include "Core.h"

#include "SceneManager.h"
#include "ComponentManager.h"
#include "GameObjectManager.h"
#include "Timer.h"
#include "UIText.h"
#include "InputManager.h"
#include "AnimatedSprite.h"
#include "PlayerControls.h"
#include "EnemyAI.h"
#include "Terrain.h"
#include "Bomb.h"
#include "Transform.h"
#include "LevelData.h"


IMPLEMENT_COMPONENT(LevelData);


void LevelData::initialize() {

    UI = NULL;
    scoreDisplay = NULL;

    bombPoolID = -1;
    firePoolID = -1;

    lives = 1;
    score = 0;

    deltaTime = 0;

    gameOver = false;
    setupPerformed = false;
}

void LevelData::onDestroy() {

    GameObjectManager::Instance().destroyObjectPool(bombPoolID);
    GameObjectManager::Instance().destroyObjectPool(firePoolID);
}


void LevelData::update() {

    if (!setupPerformed) {

        setupLevel();
    }

    if (!gameOver) {

        deltaTime += Timer::Instance().getDelta();

        if (deltaTime >= Timer::Instance().getTargetUpdatesPerSecond()) {

            deltaTime -= Timer::Instance().getTargetUpdatesPerSecond();

            // Toggle pause menu
            if (InputManager::Instance().getKeyReleased(sf::Keyboard::P)) {

                if (UI->isActive()) {

                    UI->setActive(false);

                    for (auto *go : entities) {

                        go->setYeilding(false);
                    }

                    GameObjectManager::Instance().yeildObjectPool(bombPoolID, false);
                    GameObjectManager::Instance().yeildObjectPool(firePoolID, false);
                }
                else {

                    UI->setActive(true);

                    for (auto *go : entities) {

                        go->setYeilding(true);
                    }

                    GameObjectManager::Instance().yeildObjectPool(bombPoolID, true);
                    GameObjectManager::Instance().yeildObjectPool(firePoolID, true);
                }
            }
        }
    }
    // Game over win/lose
    else {

        UI->setActive(true);

        for (auto *go : entities) {

            go->setYeilding(true);
        }

        GameObjectManager::Instance().yeildObjectPool(bombPoolID, true);
        GameObjectManager::Instance().yeildObjectPool(firePoolID, true);
    }
}

void LevelData::load(std::unique_ptr<FileSystem::FileAccessor> &accessor) {

    int bombPoolSize = 0;

    FileSystem::Instance().getAttribute(accessor, "lives", lives);

    if (FileSystem::Instance().getAttribute(accessor, "bombPoolSize", bombPoolSize)) {

        // Build a bomb fire pool for bombs to use when exploding
        GameObject *fireSample = GameObjectManager::Instance().createGameObjectFromPrefab("BombFire.prefab");

        if (fireSample != NULL) {

            fireSample->setScene(gameObject);

            firePoolID = GameObjectManager::Instance().createObjectPool(fireSample, bombPoolSize * 6);

            fireSample->destroy();
        }

        // Build the bomb pool for participating entities
        GameObject *bombSample = GameObjectManager::Instance().createGameObjectFromPrefab("Bomb.prefab");

        if (bombSample != NULL) {

            Bomb *bombS = (Bomb *)bombSample->getComponent("Bomb");

            if (bombS != NULL) {

                bombS->setFirePoolID(firePoolID);
            }

            bombSample->setScene(gameObject);

            bombPoolID = GameObjectManager::Instance().createObjectPool(bombSample, bombPoolSize);

            bombSample->destroy();
        }
    }
}

Component &LevelData::operator=(const Component &comp) {

    const LevelData *orig = (const LevelData *)&comp;

    score = orig->score;

    return *this;
}

// Perform a one time setup spawning the player and enemies and setting up object pool
void LevelData::setupLevel() {

    if (scoreDisplay = (UIText *)gameObject->getComponent("UIText")) {

        std::string text = "Score: " + std::to_string(score) + " Lives: " + std::to_string(lives);
        scoreDisplay->setText(text.c_str());
    }

    if (!(map = (Terrain *)this->gameObject->getComponent("Terrain"))) {

        map = (Terrain *)ComponentManager::Instance().createComponent("Terrain");
        gameObject->addComponent(map);
        map->loadMapFile(mapFile.c_str());
    }

    if (map != NULL) {

        if (map->spawnPoints.size() > 0) {

            std::list<std::pair<int, int>>::iterator iter = map->spawnPoints.begin();

            // Start with setting up player
            GameObject *player = GameObjectManager::Instance().createGameObjectFromPrefab("Player.prefab");

            if (player != NULL) {

                PlayerControls *pCont = (PlayerControls *)player->getComponent("PlayerControls");

                player->setScene(this->gameObject);

                if (pCont != NULL) {

                    pCont->setLevel(this);
                    pCont->setMap(map);
                    pCont->setPosition(iter->first, iter->second);
                    pCont->setBombPool(bombPoolID);
                }
                
                map->placeEntityOnMap(player, iter->first, iter->second);

                entities.push_back(player);
            }

            iter++;

            // Now spawn and setup enemies
            for (iter; iter != map->spawnPoints.end(); iter++) {

                GameObject *enemy = GameObjectManager::Instance().createGameObjectFromPrefab("Enemy.prefab");

                if (enemy != NULL) {

                    EnemyAI *eai = (EnemyAI *)enemy->getComponent("EnemyAI");

                    enemy->setScene(this->gameObject);

                    if (eai != NULL) {

                        eai->setLevel(this);
                        eai->setMap(map);
                        eai->setPosition(iter->first, iter->second);
                        eai->setBombPool(bombPoolID);
                    }

                    map->placeEntityOnMap(enemy, iter->first, iter->second);

                    entities.push_back(enemy);
                }
            }
        }

        UI = GameObjectManager::Instance().findGameObjectByName("UI");
        
        if (UI != NULL) {

            UI->setActive(false);
        }

        setupPerformed = true;
    }
}

// Reverts level to original state
void LevelData::resetLevel() {

    for (GameObject *go : entities) {

        go->destroy();
    }
    entities.clear();

    mapFile = map->getMapFile();
    map->unloadTiles();
    map->destroy();
    map = NULL;

    score = 0;

    gameOver = false;
    setupPerformed = false;
}

// Decrease lives remaining by 1
void LevelData::decreaseLives() {

    lives--;

    if (scoreDisplay != NULL) {

        std::string text = "Score: " + std::to_string(score) + " Lives: " + std::to_string(lives);
        scoreDisplay->setText(text.c_str());
    }

    if (lives <= 0) {

        if (UIText *text = (UIText *)UI->getComponent("UIText")) {

            text->setText("You lose!");
        }

        gameOver = true;
    }
    else {

        resetLevel();
    }
}

// Increase current score by X points
void LevelData::increaseScore(int points) {

    score += points;

    if (scoreDisplay != NULL) {

        std::string text = "Score: " + std::to_string(score) + " Lives: " + std::to_string(lives);
        scoreDisplay->setText(text.c_str());
    }
}

/* Gets/Sets */

unsigned int LevelData::getBombPoolID() {

    return bombPoolID;
}

// Remove entity from those still alive
void LevelData::removeEntity(GameObject *e) {

    for (auto iter = entities.begin(); iter != entities.end(); iter++) {

        if (*(*iter) == *e) {

            entities.erase(iter);
            break;
        }
    }

    // Check if last man standing is player
    if (entities.size() == 1) {

        auto iter = entities.begin();

        if (iter != entities.end()) {
            if (PlayerControls *player = (PlayerControls *)(*iter)->getComponent("PlayerControls")) {

                if (UIText *text = (UIText *)UI->getComponent("UIText")) {

                    text->setText("You win!");
                }

                gameOver = true;
            }
        }
    }
}
