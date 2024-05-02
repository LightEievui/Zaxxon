#pragma once
#include "..\AbstractBullet.h"
#include "Entity/Character/Player/Player.h"
#include "SFML/Graphics.hpp"


/// <summary>
/// The missile that the boss shoots
/// </summary>
class BossBullet : public AbstractBullet
{
public:
	BossBullet(sf::Vector3f, Entity*, sf::Texture*);
	~BossBullet();

	void update(sf::RenderWindow&);
	void damage(int);
	void collide();
	bool isDestroyed();
private:
	sf::Texture* spriteSheet;
	Entity* target;
	sf::Clock movementInt, invTimer;

	int health = 10;
};