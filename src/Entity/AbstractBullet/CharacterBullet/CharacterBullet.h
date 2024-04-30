#pragma once
#include "../AbstractBullet.h"
#include "Util/Util.h"



class CharacterBullet : public AbstractBullet
{
public:
	enum BulletDeathType {
		WallDeath,
		EnemyDeath,
		MissileDeath
	};

	/*
	Players use 3f & enemies use 2f, I know it's confusing but enemies run on 2f
	space while players are on 3f space and it's not possible to convert from 2f to 3f,
	so enemies have to pass their 2f position to the bullets. Just make sure to pass the
	2f if an enemy and the 3f will be ignored.
	*/
	CharacterBullet(sf::Texture* spritesheet, sf::Vector3f spawnPos,
		unsigned int sizeindex, BulletType = BulletType::Player, sf::Vector2f = sf::Vector2f(0,0)
	);
	~CharacterBullet();
	void kill();
	void kill(BulletDeathType type);
	void update(sf::RenderWindow& window);
	unsigned int getSizeIndex();
	sf::Vector3f getPosition()
	{

	};


private:
	unsigned int sizeIndex;
	BulletType type;
};