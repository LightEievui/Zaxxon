#include "Player.h"


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
	} // TODO: wrong texture @ [1][2]
	// 30 18 so 22 5
	this->sprite.setTextureRect(playerTextures[0][0]);
	this->setPos(sf::Vector3f(0, 69, (int)startPos*-2));
	this->shadow.setTexture(*spriteSheet);
	this->shadow.setTextureRect(sf::IntRect(352,18,22,13));
	this->shadow.setColor(sf::Color::Black);
}


void Player::update(sf::RenderWindow& window, bool inSpace)
{
	// Update texture
	unsigned int planeSizeIndex;
	unsigned int planeVertical = 0;
	getSizeIndex(planeSizeIndex);

	// Keys
	sf::Vector3f tempVelocity;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && getPos().x < xMax)
		tempVelocity.x = 1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && getPos().x > xMin)
		tempVelocity.x = -1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && getPos().y < yMax)
	{
		tempVelocity.y = 0.6f;
		planeVertical = 2;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && getPos().y > yMin)
	{
		tempVelocity.y = -0.6f;
		planeVertical = 1;
	}

	if (!inSpace)
		planeSizeIndex = 0;
	sprite.setTextureRect(playerTextures[planeVertical][planeSizeIndex]);

	// Spawn bullets
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) &&
		bulletCD.getElapsedTime().asMilliseconds() > BULLET_COOLDOWN
	)
	{
		bulletCD.restart();
		sf::Sprite temp = sf::Sprite();

		temp.setTexture(*spriteSheet);
		temp.setTextureRect(sf::IntRect(8 + 16*planeSizeIndex, 47, 16, 8));
		temp.setOrigin(0, 8);
		//temp.setPosition(sprite.getPosition().x + 22 - 2*planeSizeIndex, sprite.getPosition().y + 5 + planeSizeIndex);
		bulletsPos.push_back(sf::Vector3f(getPos().x - 21, getPos().y + .02* planeSizeIndex, getPos().z - 15));
		temp.setPosition(translateTo2d2(bulletsPos.at(bulletsPos.size() - 1)));

		bullets.push_back(temp);
	}
	#ifndef NDEBUG
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		std::cout << getPos().x << " " << getPos().y << " " << getPos().z << "\n";
	#endif

	tempVelocity.z = -2;

	// Position updates
	setVelocity(tempVelocity);
	shadow.setPosition(translateTo2d2(sf::Vector3f(getPos().x-5, 2*224 / 3, getPos().z)));

	// Drawing
	if(!inSpace)
		window.draw(shadow);

	Character::update(window); // updating position using velocity, draw character

	// Updating Bullets
	erase.clear();

	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		sf::Sprite& bullet = bullets[i];
		window.draw(bullet);
		bullet.move(translateTo2d(sf::Vector3f(0,0,-6)));

		if (!getWindowViewRect(window).intersects(bullet.getGlobalBounds()))
			erase.push_back(i);
	}

	for(unsigned int i : erase)
		bullets.erase(bullets.begin() + i);
}


void Player::kill()
{
	animations.run(this->sprite, Animation::CHARACTER_DEATH);
}


bool Player::getTranslate2()
{
	return true;
}