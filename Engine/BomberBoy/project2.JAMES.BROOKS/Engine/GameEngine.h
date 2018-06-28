#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

/*
Student: James Brooks
Class: Game Architecture

File: GameEngine.h

Class: GameEngine

Description: Holds the game loop and runs the major systems that run the game.
*/


class GameEngine {

private:
	
    GameEngine() = default;
	
    ~GameEngine() = default;
	
    GameEngine(const GameEngine& other) = default;

    // Called automatically at the end of gameLoop to clean system memory
    void clean();

public:

    inline static GameEngine& Instance() {

        static GameEngine instance;

        return instance;
    }

    // Initialize all systems needed to run the game. Call before game loop
    void initialize(const char *settingsFile);

    // Main game loop that runs the game
    void gameLoop();
};

#endif // !GAME_ENGINE_H
