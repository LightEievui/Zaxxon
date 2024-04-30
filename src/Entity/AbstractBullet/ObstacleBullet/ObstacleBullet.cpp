#include "ObstacleBullet.h"



ObstacleBullet::ObstacleBullet(sf::Vector3f pos, sf::Texture* spriteSheet, BulletType type)
{
	sprite->setTexture(*spriteSheet);
	
	if (type == BulletType::xBullet)
	{
		sprite->setTextureRect(sf::IntRect(345, 124, 14, 9));
		sprite->setOrigin(sf::Vector2f(sprite->getGlobalBounds().width,
			sprite->getGlobalBounds().height));
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


void ObstacleBullet::update(sf::RenderWindow& window)
{
	switch (type)
	{
	case BulletType::xBullet:
		setPos(sf::Vector3f(getPos().x + 3, getPos().y, getPos().z));
		break;
	case BulletType::zBullet :
		translate(3);
		break;
	}

	std::cout << window.getPosition().x;
}