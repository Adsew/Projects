#include "Display.h"
void Display::LoadSprite(std::string &d, sf::Sprite &s) {
	if (!mapTex.loadFromFile(d)) {
		std::cout << "Cannot load texture" << std::endl;
	}
	s.setTexture(mapTex);
}

Display::Display() {
	window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Heat Map");
	LoadSprite(dir, background);
}

std::string Display::GetTex() {
	return dir;
}

void Display::ShowWindow(SQLInjector &sq) {
	sf::Clock timer;
	while (window->isOpen()) {
		
        sf::Event ev;
		sf::Time elapsedTime = timer.getElapsedTime();

		while (window->pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				
				window->close();
			}
			if (ev.type == sf::Event::MouseButtonPressed) {
				SendInfo(sq, elapsedTime);
			}
		}
		window->draw(background);
		for (int i = 0; i < players.size(); i++) {
			window->draw(players[i]->GetMark());
		}
		window->display();
	}
}

void Display::SendInfo(SQLInjector &sq, sf::Time &et) {
	float t = et.asSeconds();
	sf::Vector2f pos(sf::Mouse::getPosition(*window));
	std::shared_ptr<PlayerMarker> pl(new PlayerMarker());
	
    pl->SetMarkPos(pos);
	pl->SetTimeOfDeath(t);
	
    players.push_back(pl);

    sq.sendPoint(pl->GetPos().x, pl->GetPos().y, pl->GetTimeOfDeath());
	
    std::cout << "Death occured at: " << pl->GetPos().x << ", " << pl->GetPos().y << std::endl;
	std::cout << "Time of death: " << pl->GetTimeOfDeath() << "s after start." << std::endl;
	std::cout << "Total player deaths: " << players.size() << std::endl;
}