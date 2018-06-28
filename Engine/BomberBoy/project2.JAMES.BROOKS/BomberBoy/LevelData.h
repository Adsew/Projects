#ifndef LEVEL_DATA_H
#define LEVEL_DATA_H

/*
Student: James Brooks
Class: Game Architecture

File: LevelData.h

Class: LevelData

Description: Keeps track of the entities within the current level and the score.
*/


#include "Component.h"


class Terrain;
class UIText;


class LevelData : public Component {

    REGISTER_COMPONENT(LevelData);


    /***** Variables *****/

protected:

    std::list<GameObject *> entities;
    GameObject *UI;
    UIText *scoreDisplay;

    Terrain *map;

    std::string mapFile;

    float deltaTime;

    unsigned int bombPoolID;
    unsigned int firePoolID;

    int lives;
    int score;

    bool gameOver;
    bool setupPerformed;


    /***** Functions *****/

private:

    // Perform a one time setup spawning the player and enemies and setting up object pool
    void setupLevel();

protected:

    virtual void initialize() override;

    virtual void onDestroy() override;

    virtual void update() override;

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

public:

    virtual Component &operator=(const Component &comp) override;

    // Reverts level to original state
    void resetLevel();

    // Decrease lives remaining by 1
    void decreaseLives();

    // Increase current score by X points
    void increaseScore(int points);

        /* Gets/Sets */

    unsigned int getBombPoolID();

    // Remove entity from those still alive
    void removeEntity(GameObject *e);
};

#endif // !LEVEL_DATA_H
