#include "CharacterBullet.h"


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


CharacterBullet::~CharacterBullet()
{
	delete sprite;
	sprite = nullptr;
}


unsigned int CharacterBullet::getSizeIndex()
{
	return sizeIndex;
}

void CharacterBullet::kill()
{
	kill(EnemyDeath);
}


void CharacterBullet::kill(BulletDeathType deathType)
{
	sprite->setColor(sf::Color(255, 255, 255));
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
}


void CharacterBullet::update(sf::RenderWindow& window)
{
	if (type == Player)
	{
		if (animations.getState() == 0)
			setPos(getPos() + sf::Vector3f(0, 0, -6));
		else if (animations.getState() == 2)
			setPos(getPos() + sf::Vector3f(0, 0, 0));
		sprite->setPosition(translateTo2d(getPos()));
	}
	else if (type == Enemy)
		sprite->move(translateTo2d(sf::Vector3f(0, 0, 3)));

	window.draw(*sprite);
}


bool CharacterBullet::isHit()
{
	return animations.getState() == 1;
}