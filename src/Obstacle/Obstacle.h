#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Util/Util.h"
#include <time.h>
#include "Entity/Entity.h"


class Obstacle : public Entity
{
public:
	Obstacle();

	void create(sf::Vector3f, sf::Texture*, bool = false, int = -1);
	void setTexture(std::string);
	void setPosition(sf::Vector3f);
	sf::Vector3f getPosition();
	std::vector<sf::Vector3f> getBulletLocations();
	void update(sf::RenderWindow&) override;
	bool isPresent();
	void bulletKill(int);

private:

	//Animation anim;
	std::vector<sf::Sprite> bulletSprites;
	std::vector<sf::Vector3f> bulletPositions;
	int direction, count, random;
	bool turret, onScreen = false;
};

