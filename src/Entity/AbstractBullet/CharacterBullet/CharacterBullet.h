#pragma once
#include "../AbstractBullet.h"
#include "Util/Util.h"


class CharacterBullet : public AbstractBullet
{
public:
	enum BulletType {
		Player,
		Enemy
	};

	CharacterBullet(sf::Texture* spritesheet, sf::Vector3f spawnPos, unsigned int sizeindex, BulletType = Player);
	void update(sf::RenderWindow& window);
	unsigned int getSizeIndex();
private:
	unsigned int sizeIndex;
	BulletType type;
};

