#include "Player.h"


Player::Player(sf::Texture* texture) : Character(texture)
{

}


void Player::update()
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
	Character::update(); // velocity
}


void kill()
{
	// TODO: animations.run(this->sprite, Animation::DEATH_CHARACTER);
}