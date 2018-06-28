#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

/*
Student: James Brooks
Class: Game Architecture

File: InputManager.h

Class: InputManager

Description: Manages and refines input received from the user for use elsewhere.
*/


#include <SFML/Graphics.hpp>

#include "ISystem.h"


class InputManager : public ISystem {

friend class GameEngine;

    /***** Variables *****/

private:

    std::map<sf::Keyboard::Key, bool>keyDownState;
    std::map<sf::Keyboard::Key, bool>keyPressedState;
    std::map<sf::Keyboard::Key, bool>keyReleasedState;
    std::map<sf::Mouse::Button, bool>mouseDownState;
    std::map<sf::Mouse::Button, bool>mousePressedState;
    std::map<sf::Mouse::Button, bool>mouseReleasedState;
    sf::Vector2i mousePosition;

    float deltaTime;

    bool gameCloseTriggered;


    /***** Functions *****/

private:

    InputManager() = default;

    ~InputManager() = default;

    InputManager(const InputManager& other) = default;

protected:
	
    virtual void initialize() override;
	
    virtual void clean() override;

    virtual void update() override;

public:

    inline static InputManager& Instance() {

        static InputManager instance;

        return instance;
    }

    // True if key was pressed during this update cycle
    bool getKeyPressed(const sf::Keyboard::Key &key);

    // True if key was released during this update cycle
    bool getKeyReleased(const sf::Keyboard::Key &key);

    // True if key is held down, false if up
    bool getKeyDown(const sf::Keyboard::Key &key);

    // True if button was pressed during this cycle update
    bool getMouseButtonPressed(const sf::Mouse::Button &button);

    // True if button was pressed during this cycle update
    bool getMouseButtonReleased(const sf::Mouse::Button &button);

    // True if button was pressed during this cycle update
    bool getMouseButtonDown(const sf::Mouse::Button &button);

    // Returns mouse position at the time of the cycle update
    sf::Vector2i &getMousePosition();

    // Sets the game exit flag to true (should be moved elsewhere, not sure where though)
    void exit();
};

#endif // !INPUT_MANAGER_H
