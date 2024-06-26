#include "BossBullet.h"
#include <iostream>


/// <summary>
/// Create a boss bullet based on it's position
/// </summary>
/// <param name="startPos"></param>
/// <param name="target">The entity that the missile is targetting.</param>
/// <param name="spriteSheet"></param>
BossBullet::BossBullet(sf::Vector3f startPos, Entity* target,
                       sf::Texture* spriteSheet)
{
	this->target = target;

	sprite->setTexture(*spriteSheet);
	sprite->setTextureRect(sf::IntRect(38, 74, 39, 27));
	sprite->setOrigin(sf::Vector2f(0, sprite->getGlobalBounds().height));

	sprite->setPosition(translateTo2d(startPos));

	setPos(startPos);

	movementInt.restart();
	invTimer.restart();
}


/// <summary>
/// Run logic for boss bullet then draw it to screen.
/// </summary>
/// <param name="window"></param>
void BossBullet::update(sf::RenderWindow& window, float gameSpeed)
{
	//moves toward the target(player) in a similar manner to the boss
	if (movementInt.getElapsedTime().asMilliseconds() >= 50 && animations.
		getState() == 0)
	{
		movementInt.restart();

		if ((target->getPos().x - 50) - getPos().x > 5)
			setPos(sf::Vector3f(getPos().x + 5 * gameSpeed, getPos().y,
			                    getPos().z));
		if ((target->getPos().x - 50) - getPos().x < 5)
			setPos(sf::Vector3f(getPos().x - 5 * gameSpeed, getPos().y,
			                    getPos().z));

		if (target->getPos().y - getPos().y > 3)
			setPos(sf::Vector3f(getPos().x, getPos().y + 5 * gameSpeed,
			                    getPos().z));
		if (target->getPos().y - getPos().y < 3)
			setPos(sf::Vector3f(getPos().x, getPos().y - 5 * gameSpeed,
			                    getPos().z));

		translate(3.5f);
	}

	//if the bullet dies treat it as a collision
	if (health <= 0)
		alive = false;

	window.draw(*sprite);
}


/// <summary>
/// Run the death animation and set to dead.
/// </summary>
void BossBullet::collide()
{
	if (animations.getState() == 0)
	{
		animations.run(sprite, Animation::ALT_DEATH);
	}

	alive = false;
}


/// <summary>
/// Decrease the health of the bullet
/// </summary>
/// <param name="hit"></param>
void BossBullet::damage(int hit)
{
	if (invTimer.getElapsedTime().asMilliseconds() >= 75)
	{
		invTimer.restart();
		health -= hit;
	}
}