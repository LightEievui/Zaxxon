#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Util/Util.h"
#include <time.h>
#include "Entity/Entity.h"
#include "Entity/AbstractBullet/CharacterBullet/CharacterBullet.h"


class Obstacle : public Entity
{
public:
	//For shooting obstacles
	Obstacle(sf::Vector3f, sf::Texture*, int, int);
	//For standby obstacles
	Obstacle(sf::Vector3f, sf::Texture*, int);
	~Obstacle();

	void setTexture(std::string);
	void setPosition(sf::Vector3f);
	sf::Vector3f getPosition();
	std::vector<sf::Vector3f> getBulletLocations();
	void update(sf::RenderWindow&, int playerZ);
	void update(sf::RenderWindow&) override;
	bool isPresent();
	bool isTurret();

	void bulletKill(int);
	int getType();
	int getScore();

	void kill(Animation::Anim animation = Animation::CHARACTER_DEATH);
private:
	std::vector<sf::Sprite> bulletSprites;
	std::vector<sf::Vector3f> bulletPositions;
	int direction, count = 0, total = 100, random = 0, type = 0, scoreIndicator = 0;
	bool turret = false, onScreen = false, moved = false, redRocket = false;
	const bool drawDebugText = true;
	sf::Sprite rocketExplosionSprite;
	Animation rocketAnimation;
	int rocketZ = 0;
};