#ifndef ENEMY_AI_H
#define ENEMY_AI_H

/*
Student: James Brooks
Class: Game Architecture

File: EnemyAI.h

Class: EnemyAI

Description: Controls the actions the enemies will take.
    They will move randomly and drop bombs.
*/


#include "Component.h"


class LevelData;
class Terrain;


class EnemyAI : public Component {

    REGISTER_COMPONENT(EnemyAI);


    /***** Variables *****/

    LevelData *level;
    Terrain *map;

    int posX, posY;

    unsigned int bombPoolID;

    float bombPlaceInterval;
    float bombPlaceChance;
    float moveInterval;

    float deltaTimeMove;
    float deltaTimeBomb;


    /***** Functions *****/

protected:

    virtual void initialize() override;

    virtual void update() override;

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

public:

    virtual Component &operator=(const Component &comp) override;

    // Kill the enemy and increase the score
    void kill(int score);

    void setLevel(LevelData *currentLevel);

    void setMap(Terrain *m);

    void setPosition(int x, int y);

    void setBombPool(unsigned int id);
};

#endif // !ENEMY_AI_H
