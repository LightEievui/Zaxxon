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
	Obstacle(sf::Vector3f, sf::Texture*, float, int);
	//For standby obstacles
	Obstacle(sf::Vector3f, sf::Texture*, int);
	~Obstacle();

	void setTexture(std::string);
	void setPosition(sf::Vector3f);
	sf::Vector3f getPosition();
	std::vector<sf::Vector3f> getBulletLocations();
	void update(sf::RenderWindow&) override;
	bool isPresent();
	bool isTurret();

	void bulletKill(int);
	int getType();

	void kill(Animation::Anim animation = Animation::CHARACTER_DEATH)
	{ animations.run(sprite, animation, isTurret() ? 1 : 0); };

private:
	std::vector<sf::Sprite> bulletSprites;
	std::vector<sf::Vector3f> bulletPositions;
	int direction, count = 0, total = 100, random, type;
	bool turret = false, onScreen = false, moved = false;
};