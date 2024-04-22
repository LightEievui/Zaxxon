#include "BossBullet.h"


/// <summary>
/// Create a boss bullet based on it's position
/// </summary>
/// <param name="startPos"></param>
/// <param name="target"></param>
/// <param name="spriteSheet"></param>
BossBullet::BossBullet(sf::Vector3f startPos, Entity* target, sf::Texture* spriteSheet)
{
	this->target = target;
	this->spriteSheet = spriteSheet;

	sprite->setTexture(*spriteSheet);
	sprite->setTextureRect(sf::IntRect(38, 74, 39, 27));
	sprite->setOrigin(sf::Vector2f(0, sprite->getGlobalBounds().height));

	sprite->setPosition(translateTo2d(startPos));

	setPos(startPos);
	
	movementInt.restart();
}


/// <summary>
/// Clean up memory related to boss bullet class
/// </summary>
BossBullet::~BossBullet()
{

}


/// <summary>
/// Run logic for boss bullet then draw it to screen
/// </summary>
/// <param name="window"></param>
void BossBullet::update(sf::RenderWindow& window)
{
	if (alive == true && movementInt.getElapsedTime().asMilliseconds() >= 100)
	{
		movementInt.restart();

		if ((target->getPos().x - 50) - getPos().x > 3)
			setPos(sf::Vector3f(getPos().x + 5, getPos().y, getPos().z));
		if ((target->getPos().x - 50) - getPos().x < 3)
			setPos(sf::Vector3f(getPos().x - 5, getPos().y, getPos().z));

		if (target->getPos().y - getPos().y > 3)
			setPos(sf::Vector3f(getPos().x, getPos().y + 5, getPos().z));
		if (target->getPos().y - getPos().y < 3)
			setPos(sf::Vector3f(getPos().x, getPos().y - 5, getPos().z));

		setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z + 7));
		sprite->setPosition(translateTo2d(getPos()));
	}

	if (health <= 0 && alive == true)
		collide();

	window.draw(*sprite);
}



void BossBullet::collide()
{
	alive = false;
	animations.run(sprite, Animation::ALT_DEATH);
}


bool BossBullet::isAlive()
{
	return animations.getState() == 1;
}


void BossBullet::damage(int hit)
{
	health -= hit;
}