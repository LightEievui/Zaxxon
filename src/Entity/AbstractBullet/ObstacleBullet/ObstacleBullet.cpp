#include "ObstacleBullet.h"


/// <summary>
/// Initialize based on which type of bullet this is.
/// </summary>
/// <param name="pos"></param>
/// <param name="spriteSheet"></param>
/// <param name="type"></param>
ObstacleBullet::ObstacleBullet(sf::Vector3f pos, sf::Texture* spriteSheet,
                               BulletType type)
{
	sprite->setTexture(*spriteSheet);

	//checks for the bullet direction
	switch (type)
	{
	case xBulletR:
		sprite->setTextureRect(sf::IntRect(345, 124, 14, 9));
		sprite->setOrigin(sf::Vector2f(sprite->getGlobalBounds().width,
			sprite->getGlobalBounds().height));
		break;
	case xBulletL:
		sprite->setTextureRect(sf::IntRect(345, 124, 14, 9));
		sprite->setOrigin(sf::Vector2f(0, 0));
		break;
	case zBullet:
		sprite->setTextureRect(sf::IntRect(160, 127, 14, 9));
		sprite->setOrigin(sf::Vector2f(0, sprite->getGlobalBounds().height));
		break;
	default:
		break;
	}

	setPos(pos);
	sprite->setPosition(translateTo2d(getPos()));

	this->type = type;
}


/// <summary>
/// Delete the sprite when bullet is finished.
/// </summary>
ObstacleBullet::~ObstacleBullet()
{
	delete sprite;
	sprite = nullptr;
}


/// <summary>
/// Run the logic and draw this bullet.
/// </summary>
/// <param name="window"></param>
/// <param name="gameSpeed"></param>
void ObstacleBullet::update(sf::RenderWindow& window, float gameSpeed)
{
	//change movement based on type
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
		break;
	}

	window.draw(*sprite);
}
