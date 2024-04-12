#include "BossBullet.h"


BossBullet::BossBullet(sf::Vector3f startPos, Player* target, sf::Texture* spriteSheet)
{
	this->target = target;

	sprite->setTexture(*spriteSheet);
	sprite->setTextureRect(sf::IntRect(38, 74, 39, 27));
	sprite->setOrigin(sf::Vector2f(0, sprite->getGlobalBounds().height));

	sprite->setPosition(translateTo2d(startPos));

	setPos(startPos);
}



void BossBullet::update(sf::RenderWindow& window)
{
	setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z + 4));
	sprite->setPosition(translateTo2d(getPos()));

	window.draw(*sprite);
}