#pragma once
#include "SFML/Graphics.hpp"
#include "Entity/AbstractBullet/AbstractBullet.h"


class ObstacleBullet : public AbstractBullet
{
public:
	ObstacleBullet(sf::Vector3f, sf::Texture*, BulletType);

	void update(sf::RenderWindow&);

private:
	BulletType type;
};

