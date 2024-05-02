#include "AbstractBullet.h"


bool AbstractBullet::isHit()
{
	return !alive;
}


void AbstractBullet::translate(float movement)
{
	setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z + (movement)));
	sprite->setPosition(translateTo2d(getPos()));
}