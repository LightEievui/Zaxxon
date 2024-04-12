#pragma once
#include "..\AbstractBullet.h"
#include "Entity/Character/Player/Player.h"
#include "SFML/Graphics.hpp"


class BossBullet : public AbstractBullet
{
public:
	BossBullet(sf::Vector3f, Player*);
	~BossBullet();

private:
	sf::Texture* spriteSheet;
	Player* target;

};

