#pragma once
#include "..\AbstractBullet.h"
#include "Entity/Character/Player/Player.h"
#include "SFML/Graphics.hpp"


class BossBullet : public AbstractBullet
{
public:
	BossBullet(sf::Vector3f, Entity*, sf::Texture*);
	~BossBullet();

	void update(sf::RenderWindow&);
	void collide();
	bool isAlive();
	void damage(int);

private:
	sf::Texture* spriteSheet;
	Entity* target;
	sf::Clock movementInt;
	bool alive = true;
	int health = 10;
};

