#include "Boss.h"


Boss::Boss(sf::Vector3f start, Entity* target, sf::Texture* spriteSheet) : Entity()
{
	setPos(start);

	body.setPosition(translateTo2d(start));
	body.setTexture(*spriteSheet);
	body.setTextureRect(sf::IntRect(0, 0, 58, 75));
	body.setOrigin(sf::Vector2f(body.getGlobalBounds().width / 2, body.getGlobalBounds().height / 2));

	this->target = target;
	
	movementInt.restart();
}


Boss::~Boss() 
{
}


void Boss::update(sf::RenderWindow& window)
{
	if (movementInt.getElapsedTime().asMilliseconds() >= 200)
	{
		movementInt.restart();

		if (target->getPos().x - getPos().x > 5)
			setPos(sf::Vector3f(0, 0, 0));
	}
}
