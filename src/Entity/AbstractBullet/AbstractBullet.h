#pragma once
#include "../Entity.h"


class AbstractBullet : public Entity
{
public:
	enum BulletType {
		Player,
		Enemy,
		Missile,
		zBullet,
		xBullet
	};

	AbstractBullet() : Entity() {}

	bool isHit();
	void translate(float);

protected:
	bool alive = true;
};