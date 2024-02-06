#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Util/Util.h"
#include <time.h>


class Obstacle
{
public:
	Obstacle();
	~Obstacle();

	void createObstacle(sf::Vector3f, std::string, bool = false, int = -1);
	void setTexture(std::string);
	void setPosition(sf::Vector3f);
	sf::Vector3f getPosition();
	std::vector<sf::Vector3f> getBulletLocations();
	void drawObstacle(sf::RenderWindow&);

private:

	//Animation anim;
	sf::Texture obstacleTexture, bulletTexture;
	sf::Sprite obstacleSprite;
	std::vector<sf::Sprite> bulletSprites;
	int direction, count, random;
	bool turret;
};

