#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


class Obstacle
{
public:
	Obstacle();
	~Obstacle();

	void createObstacle(sf::Vector3f, std::string, float scale = 1., bool = false, sf::Vector2f = sf::Vector2f(0, 0));
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

