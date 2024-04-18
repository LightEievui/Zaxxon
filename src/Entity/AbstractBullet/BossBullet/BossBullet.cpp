#include "BossBullet.h"


BossBullet::BossBullet(sf::Vector3f startPos, Entity* target, sf::Texture* spriteSheet)
{
	this->target = target;

	sprite->setTexture(*spriteSheet);
	sprite->setTextureRect(sf::IntRect(38, 74, 39, 27));
	sprite->setOrigin(sf::Vector2f(0, sprite->getGlobalBounds().height));

	sprite->setPosition(translateTo2d(startPos));

	setPos(startPos);
	
	movementInt.restart();
}


BossBullet::~BossBullet()
{}


void BossBullet::update(sf::RenderWindow& window)
{
	if (movementInt.getElapsedTime().asMilliseconds() >= 100)
	{
		movementInt.restart();

		if ((target->getPos().x - 50) - getPos().x > 1)
			setPos(sf::Vector3f(getPos().x + 2, getPos().y, getPos().z));
		if ((target->getPos().x - 50) - getPos().x < 1)
			setPos(sf::Vector3f(getPos().x - 2, getPos().y, getPos().z));

		if (target->getPos().y - getPos().y > 1)
			setPos(sf::Vector3f(getPos().x, getPos().y + 2, getPos().z));
		if (target->getPos().y - getPos().y < 1)
			setPos(sf::Vector3f(getPos().x, getPos().y - 2, getPos().z));

		setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z + 7));
		sprite->setPosition(translateTo2d(getPos()));
	}

	window.draw(*sprite);
}