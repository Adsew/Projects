#ifndef BUTTON_ACTION_QUIT_H
#define BUTTON_ACTION_QUIT_H

/*
Student: James Brooks
Class: Game Architecture

File: ButtonActionQuit.h

Class: ButtonActionQuit

Description: Button action that quits the game on click
*/


#include "ButtonAction.h"


class ButtonActionQuit : public ButtonAction {

    REGISTER_COMPONENT(ButtonActionQuit);

    
    /***** Variables *****/

protected:

    bool functionSet;


    /***** Functions *****/

public:

    virtual void onClick() override;
};

#endif // !BUTON_ACTION_QUIT
