#include "Character.h"


Character::Character(sf::Texture* spriteSheet)
{
	this->spriteSheet = spriteSheet;
}


void Character::update()
{
	// update the character's position using it's velocity
	int x = this->velocity.x*0.5;
	int y = this->velocity.y + this->velocity.x * 0.5;
	int z = this->velocity.z; // cant move this direction
	this->position += sf::Vector3f(x, y, z);
}


std::vector<sf::Sprite>& Character::getBullets()
{
	return bullets;
}


sf::Vector3f Character::getPos()
{
	return position;
}


void Character::setBullet(sf::IntRect bulletTexture)
{
	this->bulletTexture = bulletTexture;
}


void Character::setVelocity(sf::Vector3f vel)
{
	this->velocity = vel;
}