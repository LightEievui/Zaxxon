#include "AbstractBullet.h"


/// <summary>
/// Returns whether the bullet is dead or not.
/// </summary>
/// <returns></returns>
bool AbstractBullet::isHit()
{
	return !alive;
}


/// <summary>
/// Move a certain amount on the Z axis.
/// </summary>
/// <param name="movement"></param>
void AbstractBullet::translate(float movement)
{
	setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z + (movement)));
	sprite->setPosition(translateTo2d(getPos()));
}
