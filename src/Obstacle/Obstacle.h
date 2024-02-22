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
	//For shooting obstacles
	Obstacle(sf::Vector3f, sf::Texture*, float,  int);
	//For standby obstacles
	Obstacle(sf::Vector3f, sf::Texture*, int);
	~Obstacle();

	void setTexture(std::string);
	void setPosition(sf::Vector3f);;
	sf::Vector3f getPosition();
	std::vector<sf::Vector3f> getBulletLocations();
	void update(sf::RenderWindow&) override;
	bool isPresent();
	void bulletKill(int);

private:

	std::vector<sf::Sprite> bulletSprites;
	std::vector<sf::Vector3f> bulletPositions;
	int direction, count, total = 100, random;
	bool turret = false, onScreen = false;
};

