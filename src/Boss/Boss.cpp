#include "Boss.h"


/// <summary>
/// Prepare the boss variables
/// </summary>
/// <param name="start"></param>
/// <param name="target"></param>
/// <param name="bossSheet"></param>
/// <param name="spriteSheet"></param>
Boss::Boss(sf::Vector3f start, Entity* target, sf::Texture* bossSheet, sf::Texture* spriteSheet) : Entity()
{
	setPos(start);

	sprite->setPosition(translateTo2d(start));
	sprite->setTexture(*bossSheet);
	sprite->setTextureRect(sf::IntRect(0, 0, 58, 75));
	sprite->setOrigin(sf::Vector2f(sprite->getGlobalBounds().width/2, sprite->getGlobalBounds().height/2));

	this->target = target;

	this->spriteSheet = *spriteSheet;
	
	movementInt.restart();
}


/// <summary>
/// Clean up memory related to the boss
/// </summary>
Boss::~Boss() 
{
}


/// <summary>
/// Run the logic for the boss each frame and then draw it to the screen
/// </summary>
/// <param name="window"></param>
void Boss::update(sf::RenderWindow& window)
{
	if (movementInt.getElapsedTime().asMilliseconds() >= 100)
	{
		movementInt.restart();

		if (target->getPos().x - getPos().x > 1)
			setPos(sf::Vector3f(getPos().x+3, getPos().y, getPos().z));

		if (target->getPos().x - getPos().x < 1)
			setPos(sf::Vector3f(getPos().x - 3, getPos().y, getPos().z));

		if (abs(getPos().z - target->getPos().z) > 200)
		{
			if (stages == 0)
			{
				setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z + 3));
			}
			else
			{
				setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z - 7));
			}
		}
		else
		{
			sprite->setTextureRect(sf::IntRect(58, 0, 58, 75));
			stages++;
			setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z - 7));
		}

		sprite->setPosition(translateTo2d(getPos()));
	}


	window.draw(*sprite);
}