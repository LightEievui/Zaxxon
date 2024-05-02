#pragma once
#include "SFML/Graphics.hpp"
#include "../AbstractBullet.h"


class ObstacleBullet : public AbstractBullet
{
public:
	ObstacleBullet(sf::Vector3f, sf::Texture*, BulletType);
	~ObstacleBullet();

	void update(sf::RenderWindow&) override;

private:
	BulletType type;
};
