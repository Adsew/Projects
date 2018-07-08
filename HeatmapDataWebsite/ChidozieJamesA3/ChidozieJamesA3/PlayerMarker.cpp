#include "PlayerMarker.h"

PlayerMarker::PlayerMarker() {
	pos = sf::Vector2f(0, 0);
}

sf::CircleShape PlayerMarker::GetMark() {
	return marker;
}

sf::Vector2f PlayerMarker::GetPos() {
	return pos;
}

float PlayerMarker::GetTimeOfDeath() {
	return deathTime;
}

void PlayerMarker::SetMarkPos(sf::Vector2f &p) {
	pos = p;
	marker.setRadius(4.0f);
	marker.setOutlineThickness(2.0f);
	marker.setOutlineColor(sf::Color::Red);
	marker.setFillColor(sf::Color::Transparent);
	marker.setPosition(p);
}

void PlayerMarker::SetTimeOfDeath(float &t) {
	deathTime = t;
}

