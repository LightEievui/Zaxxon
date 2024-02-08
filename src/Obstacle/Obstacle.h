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
	~Obstacle();

	void createObstacle(sf::Vector3f, std::string, bool = false, int = -1);
	void setTexture(std::string);
	void setPosition(sf::Vector3f);
	sf::Vector3f getPosition();
	std::vector<sf::Vector3f> getBulletLocations();
	void update(sf::RenderWindow&) override;
	void bulletKill(int);

private:

	//Animation anim;
	sf::Texture bulletTexture;
	std::vector<sf::Sprite> bulletSprites;
	int direction, count, random;
	bool turret;
};

