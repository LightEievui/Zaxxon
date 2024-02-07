#include "Player.h"


Player::Player(sf::Texture* texture) : Character(texture)
{
	this->sprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); // temp player texture
	this->setPos(sf::Vector3f(20, 69, 0));
	this->shadow.setTexture(*spriteSheet);
	this->shadow.setTextureRect(sf::IntRect(352,18,22,13));
	this->shadow.setColor(sf::Color::Black);
}


void Player::update(sf::RenderWindow& window)
{
	sf::Vector3f tempVelocity;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && getPos().x < 20)
		tempVelocity.x = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && getPos().x > -113)
		tempVelocity.x = -1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && getPos().y < 135)
		tempVelocity.y = 1; // change height in gui or wtv
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && getPos().y > 69)
		tempVelocity.y = -1;

	// TODO: TEMPORARY KEYBIND, REMOVE LATER
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		kill();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) // TODO: DEBUG TEMP
		std::cout << getPos().x << " " << getPos().y << " " << getPos().z << "\n";
	// 135 y bottom limit 46 upper
	// 5 x bottom limit -113 upper

	setVelocity(tempVelocity);
	shadow.setPosition(translateTo2d(sf::Vector3f(getPos().x-21, 2*224 / 3, getPos().z)));
	window.draw(shadow);
	Character::update(window); // velocity, draw character
}


void Player::kill()
{
	animations.run(this->sprite, Animation::CHARACTER_DEATH);
}


bool Player::getMoveWithView()
{
	return true;
}