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

	sprite.setPosition(getTranslate2() ? translateTo2d2(position) : translateTo2d(position));
	window.draw(sprite);
}


std::vector<sf::Sprite>& Character::getBullets()
{
	return bullets;
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