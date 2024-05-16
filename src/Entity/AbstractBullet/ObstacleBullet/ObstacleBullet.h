#pragma once
#include "SFML/Graphics.hpp"
#include "../AbstractBullet.h"


class ObstacleBullet : public AbstractBullet
{
public:
	ObstacleBullet(sf::Vector3f, sf::Texture*, BulletType);

	void update(sf::RenderWindow&, float) override;
private:
	BulletType type;
};
