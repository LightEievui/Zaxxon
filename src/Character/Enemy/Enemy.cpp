#include "Enemy.h"


Enemy::Enemy(sf::Texture* texture, unsigned int id) : Character(texture)
{
	for (unsigned int i = 0; i < 2; i++)
		for (unsigned int j = 0; j < 4; j++)
			textures[i][j] = sf::IntRect(96 + 25*(i+j), 37, 25, 25);
	this->sprite.setTextureRect(textures[0][0]);

	this->id = id;
	alive.restart();

	switch (id)
	{
	case 0:
		this->setPos(sf::Vector3f(-200, 0, -500));
	}
}


void Enemy::update(sf::RenderWindow& window)
{
	
	runAI();
	unsigned int planeSizeIndex;
	unsigned int planeVertical = 0;
	getSizeIndex(planeSizeIndex);

	sf::Vector3f vel = getVelocity();
	if (vel.y > 0)
		planeVertical = 1;

	sprite.setTextureRect(textures[planeVertical][planeSizeIndex]);
	Character::update(window);
}


void Enemy::kill()
{

}


void Enemy::runAI()
{
	sf::Vector3f vel;
	sf::Int32 msPassed = alive.getElapsedTime().asMilliseconds();

	switch (id)
	{
	case 0:
		if (msPassed%2000 < 1000)
		{
			vel.y = -0.3;
			vel.z = 0.3;
		}
		else
		{
			vel.y = 0.3;
			vel.z = -0.3;
		}
		break;
	}

	if ((getPos().x < xMin && vel.x < 0) || (getPos().x > xMax && vel.x > 0))
		vel.x = 0;
	else if ((getPos().y < yMin && vel.y < 0) || (getPos().y > yMax && vel.y > 0))
		vel.y = 0;

	Character::setVelocity(vel);
}