#pragma once
#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "SQLInjector.h"

class PlayerMarker {
private:
	sf::Vector2f pos;
	sf::CircleShape marker;
	float deathTime;

public:
	PlayerMarker();
	sf::CircleShape GetMark();
	sf::Vector2f GetPos();
	float GetTimeOfDeath();
	void SetMarkPos(sf::Vector2f &p);
	void SetTimeOfDeath(float &t);
	~PlayerMarker() {};
};