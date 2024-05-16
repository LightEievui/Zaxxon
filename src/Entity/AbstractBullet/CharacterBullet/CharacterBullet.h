#pragma once
#include "../AbstractBullet.h"
#include "Util/Util.h"


/// <summary>
/// The bullet used by the player and enemies
/// </summary>
class CharacterBullet : public AbstractBullet
{
public:
	enum BulletDeathType
	{
		WallDeath,
		EnemyDeath,
		MissileDeath
	};

	CharacterBullet(sf::Texture* spritesheet, sf::Vector3f spawnPos,
	                unsigned int sizeindex, BulletType = Player,
	                sf::Vector2f = sf::Vector2f(0, 0)
	);

	void kill();
	void kill(BulletDeathType type);
	void update(sf::RenderWindow& window, float) override;
	unsigned int getSizeIndex();
private:
	unsigned int sizeIndex;
	BulletType type;
};
