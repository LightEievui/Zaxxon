#include "AbstractBullet.h"


/// <summary>
/// Returns the dead/alive state of the bullet via a bool
/// </summary>
/// <returns></returns>
bool AbstractBullet::isHit()
{
	return !alive;
}


/// <summary>
/// Moves the bullet by some vector in the Z
/// </summary>
/// <param name="movement"></param>
void AbstractBullet::translate(float movement)
{
	//moves along the z axis, a functionality for all bullets
	setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z + (movement)));
	sprite->setPosition(translateTo2d(getPos()));
}
