#include "ObstacleBullet.h"


ObstacleBullet::ObstacleBullet(sf::Vector3f pos, sf::Texture* spriteSheet, BulletType type)
{
	sprite->setTexture(*spriteSheet);
	
	if (type == BulletType::xBulletR)
	{
		sprite->setTextureRect(sf::IntRect(345, 124, 14, 9));
		sprite->setOrigin(sf::Vector2f(sprite->getGlobalBounds().width,
			sprite->getGlobalBounds().height));
	}
	else if (type == BulletType::xBulletL)
	{
		sprite->setTextureRect(sf::IntRect(345, 124, 14, 9));
		sprite->setOrigin(sf::Vector2f(0,0));
	}
	else if (type == BulletType::zBullet)
	{
		sprite->setTextureRect(sf::IntRect(160, 127, 14, 9));
		sprite->setOrigin(sf::Vector2f(0, sprite->getGlobalBounds().height));
	}
	
	setPos(pos);
	sprite->setPosition(translateTo2d(getPos()));

	this->type = type;
}


ObstacleBullet::~ObstacleBullet()
{
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
}


void ObstacleBullet::update(sf::RenderWindow& window)
{

	switch (type)
	{
	case BulletType::xBulletL:
		setPos(sf::Vector3f(getPos().x + 3, getPos().y, getPos().z));
		sprite->setPosition(translateTo2d(getPos()));
		break;
	case BulletType::xBulletR:
		setPos(sf::Vector3f(getPos().x - 3, getPos().y, getPos().z));
		sprite->setPosition(translateTo2d(getPos()));
		break;
	case BulletType::zBullet :
		translate(3);
		break;
	}

	window.draw(*sprite);
}