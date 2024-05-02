#pragma once
#include "../Entity.h"


/// <summary>
/// Class to hold information that the other bullets share.
/// </summary>
class AbstractBullet : public Entity
{
public:
	enum BulletType {
		Player,
		Enemy,
		Missile,
		zBullet,
		xBulletL,
		xBulletR
	};

	AbstractBullet() : Entity() {}

	bool isHit();
	void translate(float);

protected:
	bool alive = true;
};