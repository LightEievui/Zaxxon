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
	~CharacterBullet();
	void kill();
	void update(sf::RenderWindow& window);
	unsigned int getSizeIndex();
	sf::Vector3f getPosition()
	{

	};
	bool isHit();

private:
	unsigned int sizeIndex;
	BulletType type;
};