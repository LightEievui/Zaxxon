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


void Character::getSizeIndex(unsigned int& planeSizeIndex)
{
	const int y = (int)getPos().y - ((float)yMax);
	const int qSize = (yMin - yMax) / 4;
	planeSizeIndex = 3;

	for (int i = 0; i < 4; i++)
		if (y < qSize * i && y < qSize * i + 1)
			planeSizeIndex = 3 - i;
}


sf::Vector3f Character::getVelocity()
{
	return velocity;
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