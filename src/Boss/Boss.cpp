#include "Boss.h"


Boss::Boss(sf::Vector3f start, Entity* target, sf::Texture* spriteSheet) : Entity()
{
	setPos(start);

	sprite->setPosition(translateTo2d(start));
	sprite->setTexture(*spriteSheet);
	sprite->setTextureRect(sf::IntRect(0, 0, 58, 75));
	sprite->setOrigin(sf::Vector2f(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2));

	this->target = target;
	
	movementInt.restart();
}


Boss::~Boss() 
{
}


void Boss::update(sf::RenderWindow& window)
{
	std::cout << "\n" << getPos().x << ", " << getPos().y << ", " << getPos().z;
	if (movementInt.getElapsedTime().asMilliseconds() >= 100)
	{
		movementInt.restart();

		if (target->getPos().x - getPos().x > 5)
			setPos(sf::Vector3f(getPos().x+5, getPos().y, getPos().z));

		if (target->getPos().x - getPos().x < 5)
			setPos(sf::Vector3f(getPos().x - 5, getPos().y, getPos().z));

		sprite->setPosition(translateTo2d(getPos()));
	}

	window.draw(*sprite);
}
