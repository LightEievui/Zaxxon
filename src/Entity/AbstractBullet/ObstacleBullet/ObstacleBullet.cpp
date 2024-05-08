#include "ObstacleBullet.h"

/// <summary>
/// Constructor for ObstacleBullet.
/// </summary>
/// <param name="pos"></param>
/// <param name="spriteSheet"></param>
/// <param name="type"></param>
ObstacleBullet::ObstacleBullet(sf::Vector3f pos, sf::Texture* spriteSheet,
                               BulletType type)
{
	sprite->setTexture(*spriteSheet);

	if (type == xBulletR)
	{
		sprite->setTextureRect(sf::IntRect(345, 124, 14, 9));
		sprite->setOrigin(sf::Vector2f(sprite->getGlobalBounds().width,
		                               sprite->getGlobalBounds().height));
	}
	else if (type == xBulletL)
	{
		sprite->setTextureRect(sf::IntRect(345, 124, 14, 9));
		sprite->setOrigin(sf::Vector2f(0, 0));
	}
	else if (type == zBullet)
	{
		sprite->setTextureRect(sf::IntRect(160, 127, 14, 9));
		sprite->setOrigin(sf::Vector2f(0, sprite->getGlobalBounds().height));
	}

	setPos(pos);
	sprite->setPosition(translateTo2d(getPos()));

	this->type = type;
}


/// <summary>
/// Update the ObstacleBullet.
/// </summary>
/// <param name="window"></param>
/// <param name="gameSpeed"></param>
void ObstacleBullet::update(sf::RenderWindow& window, float gameSpeed)
{
	switch (type)
	{
	case xBulletL:
		setPos(sf::Vector3f(getPos().x + 3 * gameSpeed, getPos().y,
		                    getPos().z));
		sprite->setPosition(translateTo2d(getPos()));
		break;
	case xBulletR:
		setPos(sf::Vector3f(getPos().x - 3 * gameSpeed, getPos().y,
		                    getPos().z));
		sprite->setPosition(translateTo2d(getPos()));
		break;
	case zBullet:
		translate(3 * gameSpeed);
	}

	window.draw(*sprite);
}
