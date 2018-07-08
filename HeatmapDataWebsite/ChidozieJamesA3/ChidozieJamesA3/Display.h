#pragma once
#include "PlayerMarker.h"

class Display {
private:
	std::unique_ptr<sf::RenderWindow> window;
	sf::Sprite background;
	std::vector<std::shared_ptr<PlayerMarker>> players;
	std::string dir = "../Resources/Assignment2.png";
	sf::Texture mapTex;
public:
	Display();
	std::string GetTex();
	void LoadSprite(std::string &d, sf::Sprite &s);
	void ShowWindow(SQLInjector &sq);
	void SendInfo(SQLInjector &sq, sf::Time &et);
	~Display() {};
};