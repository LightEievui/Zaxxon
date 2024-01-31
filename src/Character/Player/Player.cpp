#include "Player.h"


Player::Player(sf::Texture* texture) : Character(texture)
{
	this->sprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); // temp
}


void Player::update(sf::RenderWindow& window)
{
	sf::Vector3f tempVelocity;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		tempVelocity.x = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		tempVelocity.x = -1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		tempVelocity.y = 1; // change height in gui or wtv
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		tempVelocity.y = -1;



	setVelocity(tempVelocity);
	Character::update(window); // velocity
}


void Player::kill()
{
	// TODO: animations.run(this->sprite, Animation::DEATH_CHARACTER);
}