#ifndef I_SYSTEM_H
#define I_SYSTEM_H

/*
Student: James Brooks
Class: Game Architecture

File: GameEngine.cpp

Class: GameEngine

Description: Holds the game loop and runs the major systems that run the game.
*/


class ISystem {

    
    /***** Functions *****/

protected:			
	
    virtual void initialize() = 0;

    virtual void clean() = 0;
	
	virtual void update() = 0;
};

#endif // !I_SYSTEM_H
