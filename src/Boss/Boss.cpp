#include "Boss.h"


Boss::Boss(sf::Vector3f start, Entity* target, sf::Texture* bossSheet, sf::Texture* spriteSheet) : Entity()
{
	setPos(start);

	sprite->setPosition(translateTo2d(start));
	sprite->setTexture(*spriteSheet);
	sprite->setTextureRect(sf::IntRect(0, 0, 58, 75));
	sprite->setOrigin(sf::Vector2f(spriteSheet->getSize().x/2, 0));

	this->target = target;

	this->spriteSheet = *spriteSheet;
	
	movementInt.restart();
}


Boss::~Boss() 
{
}


void Boss::update(sf::RenderWindow& window)
{
	if (movementInt.getElapsedTime().asMilliseconds() >= 100)
	{
		//stages++;
		movementInt.restart();

		if (target->getPos().x - getPos().x + 50 > 5)
			setPos(sf::Vector3f(getPos().x+5, getPos().y, getPos().z));

		if (target->getPos().x - getPos().x + 50 < 5)
			setPos(sf::Vector3f(getPos().x - 5, getPos().y, getPos().z));

		if (abs(getPos().z - target->getPos().z) > 260)
		{
			setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z + 3));
		}
		else
		{
			sprite->setTextureRect(sf::IntRect(58, 0, 58, 75));
		}

		sprite->setPosition(translateTo2d(getPos()));
	}

	window.draw(*sprite);
}
