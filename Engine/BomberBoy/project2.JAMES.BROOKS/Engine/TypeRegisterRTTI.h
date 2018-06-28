#ifndef TYPE_REGISTER_RTTI
#define TYPE_REGISTER_RTTI

/*
Student: James Brooks
Class: Game Architecture

File: TypeRegisterRTTY

Description: Used by the managers to ensure registration of all
    core engine types.
*/


#include "Transform.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "UIText.h"
#include "UIButton.h"
#include "UITexture.h"
#include "ButtonActionQuit.h"
#include "ButtonActionSceneChange.h"

#include "TextureAsset.h"


static void REGISTER_CORE_COMPONENTS() {

    CORE_IMPLEMENT_COMPONENT(Transform);
    CORE_IMPLEMENT_COMPONENT(Sprite);
    CORE_IMPLEMENT_COMPONENT(AnimatedSprite);
    CORE_IMPLEMENT_COMPONENT(UIText);
    CORE_IMPLEMENT_COMPONENT(UIButton);
    CORE_IMPLEMENT_COMPONENT(UITexture);
    CORE_IMPLEMENT_COMPONENT(ButtonActionQuit);
    CORE_IMPLEMENT_COMPONENT(ButtonActionSceneChange);
}

static void REGISTER_CORE_ASSETS() {

    CORE_IMPLEMENT_ASSET(TextureAsset);
}


#endif // !TYPE_REGISTER_RTTI
