#pragma once
#include "SFML/Graphics.hpp"
#include "Entity/Entity.h"
#include "Util/Util.h"
#include "Entity/AbstractBullet/BossBullet/BossBullet.h"


class Boss : public Entity
{
public:
	Boss(sf::Vector3f, Entity*, sf::Texture*, sf::Texture*);
	~Boss();

	void update(sf::RenderWindow&);
	void hit();
	BossBullet* getMissile();
	bool missileCreated();
private:

	Entity* target;
	sf::Clock movementInt, invFrames;
	int stages = 0, hitCount = 20, hits = 0;

	sf::Texture spriteSheet;
	BossBullet* missile = nullptr;
	bool bulletCreated = false;
	sf::Vector3f targetPoints[3];
};