#ifndef BUTTON_ACTION_SCENE_CHANGE_H
#define BUTTON_ACTION_SCENE_CHANGE_H

/*
Student: James Brooks
Class: Game Architecture

File: ButtonActionSceneChange.h

Class: ButtonActionSceneChange

Description: Button action that changes scenes when clicked
*/


#include "ButtonAction.h"


class ButtonActionSceneChange : public ButtonAction {

    REGISTER_COMPONENT(ButtonActionSceneChange);


    /***** Variables *****/

protected:

    std::string nextScene;


    /***** Functions *****/

protected:

    virtual void load(std::unique_ptr<FileSystem::FileAccessor> &accessor) override;

public:

    virtual Component &operator=(const Component &comp) override;

    void setScene(const char *scene);

    virtual void onClick() override;
};

#endif // !BUTON_ACTION_SCENE_CHANGE
