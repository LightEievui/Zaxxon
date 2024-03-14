#include "Player.h"


/// <summary>
/// Create player character at position and load resources for them.
/// </summary>
/// <param name="texture"></param>
/// <param name="startPos"></param>
Player::Player(sf::Texture* texture, unsigned int startPos) : Character(texture)
{
	// 1st @ 8 13 24 32
	sf::IntRect a = sf::IntRect(8, 13, 23, 23);
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			playerTextures[i][j] = a;
			a.left += 23;
		}
	}
	this->sprite.setTextureRect(playerTextures[0][0]);
	this->setPos(sf::Vector3f(0, 69, (int)startPos * -1.33333));
	this->shadow.setTexture(*spriteSheet);
	this->shadow.setTextureRect(sf::IntRect(352,18,22,13));
	this->shadow.setColor(sf::Color::Black);

	// Prepare bullet sound
	bulletBuffer.loadFromFile("res/sfx/08.wav");
	bulletSound.setBuffer(bulletBuffer);
}


/// <summary>
/// Run all the logic and controls for player.
/// </summary>
/// <param name="window"></param>
/// <param name="inSpace"></param>
void Player::update(sf::RenderWindow& window, bool inSpace)
{
	// Update texture
	unsigned int planeVertical = 0;
	_getSizeIndex(sizeIndex);

	// Keys
	sf::Vector3f tempVelocity;
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || 
		sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -10) && 
		getPos().x < xMax)
			tempVelocity.x = 1;
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || 
		sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 10) && 
		getPos().x > xMin)
			tempVelocity.x = -1;

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || 
		sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -10) && 
		getPos().y < yMax)
	{
		tempVelocity.y = 0.6f;
		planeVertical = 2;
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || 
		sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 10) && 
		getPos().y > yMin)
	{
		tempVelocity.y = -0.6f;
		planeVertical = 1;
	}

	if (!inSpace)
		sizeIndex = 0;
	sprite.setTextureRect(playerTextures[planeVertical][sizeIndex]);

	// Spawn bullets
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || 
		sf::Joystick::isButtonPressed(0, 0)) &&
		bulletCD.getElapsedTime().asMilliseconds() > BULLET_COOLDOWN)
	{
		bulletCD.restart();
		sf::Sprite temp = sf::Sprite();

		temp.setTexture(*spriteSheet);
		temp.setTextureRect(sf::IntRect(8 + 16* sizeIndex, 47, 16, 8));
		temp.setOrigin(0, 8);
		//temp.setPosition(sprite.getPosition().x + 22 - 2*sizeIndex, sprite.getPosition().y + 5 + sizeIndex);
		bulletsPos.push_back(sf::Vector3f(getPos().x - 21, getPos().y + .02* sizeIndex, getPos().z - 15));
		temp.setPosition(translateTo2d(bulletsPos.at(bulletsPos.size() - 1)));

		bullets.push_back(temp);

		bulletSound.play();
	}
	#ifndef NDEBUG
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		std::cout << getPos().x << " " << getPos().y << " " << getPos().z << "\n";
	#endif

	//tempVelocity.z = -2;
	tempVelocity.z = -1.3; //for translateTo2d
	
	// Position updates
	setVelocity(tempVelocity);
	shadow.setPosition(translateTo2d(sf::Vector3f(getPos().x-5, 2*224 / 3, getPos().z)));

	// Drawing
	if(!inSpace)
		window.draw(shadow);

	Character::update(window); // updating position using velocity, draw character
	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		sf::Sprite& bullet = bullets[i];
		window.draw(bullet);
		bullet.move(translateTo2d(sf::Vector3f(0,0,-6)));
		bulletsPos.at(i).z -= 6;

		if (!getWindowViewRect(window).intersects(bullet.getGlobalBounds()))
		{
			bullets.erase(bullets.begin() + i);
			bulletsPos.erase(bulletsPos.begin() + i);
			i--;
		}
	}		
}


/// <summary>
/// Run animations and other code for when player dies.
/// </summary>
void Player::kill()
{
	animations.run(this->sprite, Animation::CHARACTER_DEATH);
	//Not perfect but works
	setPos(sf::Vector3f(0, 69, getPos().z));
}


/// <summary>
/// Reset the players position to default.
/// </summary>
/// <param name="zOffset"></param>
void Player::resetPos(int zOffset)
{
	setPos(sf::Vector3f(getPos().x, getPos().y, zOffset * -1.33333));
}