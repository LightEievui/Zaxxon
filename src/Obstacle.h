#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


class Obstacle
{
public:
	Obstacle(sf::Vector3f, std::string, bool = false);
	~Obstacle();

	void setTexture(std::string);
	void setPosition(sf::Vector3f);
	sf::Vector3f getPosition();
	std::vector<sf::Vector3f> getBulletLocations();

private:

	//Animation anim;
	sf::Texture obstacleTexture, bulletTexture;
	sf::Sprite obstacleSprite;
	std::vector<sf::Sprite> bulletSprites;
};

