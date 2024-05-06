#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Util/Util.h"
#include <time.h>
#include "Entity/Entity.h"
#include "Entity/AbstractBullet/ObstacleBullet/ObstacleBullet.h"


/// <summary>
/// Still obstacles that spawn across the levels.
/// </summary>
class Obstacle : public Entity
{
public:
	/*
	KEY
	1 = gas can
	2 = satellite
	3 = grey cannon
	4 = green cannon
	5 = Shooting Up
	6 = Plane
	7 = blue floating gas can
	8 = green shooting right
	*/

	enum ObstacleType
	{
		GAS_CAN = 1,
		SATELLITE,
		GREY_CANNON,
		GREEN_CANNON,
		MISSILE_UP,
		PLANE,
		SPACE_FUEL,
		GREEN_CANNON_RIGHT
	};

	//For shooting obstacles
	Obstacle(sf::Vector3f, sf::Texture*, int, int);
	//For standby obstacles
	Obstacle(sf::Vector3f, sf::Texture*, int);
	~Obstacle() override;

	void setPosition(sf::Vector3f);
	sf::Vector3f getPosition();
	std::vector<sf::Vector3f> getBulletLocations();
	void update(sf::RenderWindow&, int playerZ, float);
	void update(sf::RenderWindow&, float) override;
	bool isPresent();
	bool isTurret();

	void bulletKill(int);
	ObstacleType getType();
	int getScore();

	void kill(Animation::Anim animation = Animation::CHARACTER_DEATH) override;

private:
	std::vector<ObstacleBullet*> bullets;
	std::vector<sf::Vector3f> bulletPositions;
	int direction, count = 1, total = 100, scoreIndicator = 0;
	ObstacleType type;
	bool turret = false, onScreen = false, moved = false, redRocket = false;
	const bool drawDebugText = true;
	sf::Sprite rocketExplosionSprite;
	Animation rocketAnimation;
	int rocketZ = 0;
};
