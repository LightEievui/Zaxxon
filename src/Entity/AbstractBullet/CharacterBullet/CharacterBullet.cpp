#include "CharacterBullet.h"


/// <summary>
/// Create a bullet based on if player or enemy shot it
/// </summary>
/// <param name="spritesheet"></param>
/// <param name="spawnPos"></param>
/// <param name="sizeIndex"></param>
/// <param name="type"></param>
/// <param name="spawnPos2f"></param>
CharacterBullet::CharacterBullet(sf::Texture* spritesheet, sf::Vector3f spawnPos,
	unsigned int sizeIndex, BulletType type, sf::Vector2f spawnPos2f
)
{
	sprite->setTexture(*spritesheet);
	sprite->setTextureRect(sf::IntRect(8 + 16 * sizeIndex, 47, 16, 8));
	sprite->setOrigin(0, 8);

	if (type == Player)
	{
		sprite->setColor(sf::Color(0, 222, 0));
		setPos(sf::Vector3f(spawnPos.x - 21.f, spawnPos.y + .02f * sizeIndex, spawnPos.z - 15.f));
		sprite->setPosition(translateTo2d(getPos()));
	}
	else if (type == Enemy)
	{
		sprite->setColor(sf::Color(222, 0, 0));
		sprite->setPosition(spawnPos2f + sf::Vector2f(-3.f * (int)(3U - sizeIndex), 20));
	}
	this->sizeIndex = sizeIndex;
	this->type = type;
}


/// <summary>
/// Clean up memory related to character bullet
/// </summary>
CharacterBullet::~CharacterBullet()
{
	delete sprite;
	sprite = nullptr;
}


/// <summary>
/// Get public size index of this character bullet
/// </summary>
/// <returns></returns>
unsigned int CharacterBullet::getSizeIndex()
{
	return sizeIndex;
}


/// <summary>
/// Default kill, run the kill method with enemyDeath as parameter
/// </summary>
void CharacterBullet::kill()
{
	kill(EnemyDeath);
}


/// <summary>
/// The bullet hit something, run code to kill bullet based on what it hit.
/// </summary>
/// <param name="deathType"></param>
void CharacterBullet::kill(BulletDeathType deathType)
{
	sprite->setColor(sf::Color( 255, 255, 255 ));
	switch (deathType)
	{
	case BulletDeathType::WallDeath:
		if (animations.getState() < 1)
			animations.run(sprite, Animation::Anim::WALLBULLET_DEATH, sizeIndex);
		break;
	case BulletDeathType::EnemyDeath:
		// adjust position to align better
		if (this->type == Player)
			setPos(getPos() + sf::Vector3f(0, 0, -16));
		if (animations.getState() < 2)
			animations.run(sprite, Animation::Anim::BULLET_DEATH, sizeIndex);
		break;
	default:
		break;
	}

	alive = false;
}


/// <summary>
/// Run the logic for this bullet and then draw it to screen
/// </summary>
/// <param name="window"></param>
void CharacterBullet::update(sf::RenderWindow& window)
{
	if (type == Player)
	{
		if (animations.getState() == 0)
			translate(-6);
	}
	else if (type == Enemy)
		translate(3);

	window.draw(*sprite);
}
