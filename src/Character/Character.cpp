#include "Character.h"


Character::Character(sf::Texture* spriteSheet) : Entity()
{
	this->spriteSheet = spriteSheet;
	this->sprite.setTexture(*this->spriteSheet);
}


void Character::update(sf::RenderWindow& window)
{
	// update the character's position using it's velocity
	
	this->position += this->velocity;

	sprite.setPosition(translateTo2d(position));
	window.draw(sprite);
}


std::vector<sf::Sprite>& Character::getBullets()
{
	return bullets;
}


bool Character::getMoveWithView()
{
	return false;
}


void Character::setPos(sf::Vector3f pos)
{
	position = pos;
}


void Character::setBullet(sf::IntRect bulletTexture)
{
	this->bulletTexture = bulletTexture;
}


void Character::setVelocity(sf::Vector3f vel)
{
	this->velocity = vel;
}