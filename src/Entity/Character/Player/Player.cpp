#include "Player.h"
#include "Background/Background.h"


/// <summary>
/// Create player character at position and load resources for them.
/// </summary>
/// <param name="texture"></param>
/// <param name="startPos"></param>
Player::Player(sf::Texture* texture, unsigned int startPos) : Character(texture)
{
	// 1st @ 8 13 24 32
	auto a = sf::IntRect(8, 13, 23, 23);
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			playerTextures[i][j] = a;
			a.left += 23;
		}
	}
	this->sprite->setTextureRect(playerTextures[0][0]);
	this->setPos(sf::Vector3f(0, 69, static_cast<int>(startPos) * -1.33333f));
	this->shadow.setTexture(*texture);
	this->shadow.setTextureRect(sf::IntRect(352, 18, 22, 13));
	this->shadow.setColor(sf::Color::Black);
	this->hitmarker.setTexture(*texture);
	this->hitmarker.setTextureRect(sf::IntRect(320, 84, 12, 11));

	// Prepare bullet sound
	bulletBuffer.loadFromFile("res/sfx/08.wav");
	bulletSound.setBuffer(bulletBuffer);
}


/// <summary>
/// Run all the logic and controls for player.
/// </summary>
/// <param name="window"></param>
/// <param name="inSpace"></param>
void Player::update(sf::RenderWindow& window, int stage, float gameSpeed)
{
	if (!alive)
		return;

	// If alive after this point
	// Update texture
	unsigned int planeVertical = 0;
	_getSizeIndex(sizeIndex);

	// Keys
	sf::Vector3f tempVelocity = getVelocity();
	constexpr float acceleration = 1.f / 5.f;

	if (leftPressed() && getPos().x < xMax && tempVelocity.x < 1.f)
		tempVelocity.x += acceleration;
	else if (rightPressed() && getPos().x > xMin && tempVelocity.x > -1.f)
		tempVelocity.x -= acceleration;
	else if (tempVelocity.x < 0)
		tempVelocity.x += acceleration;
	else if (tempVelocity.x > 0)
		tempVelocity.x -= acceleration;

	if (tempVelocity.x < acceleration && tempVelocity.x > -acceleration)
		tempVelocity.x = 0;

	if (upPressed() && tempVelocity.y < 0.6f)
		tempVelocity.y += acceleration / 0.5f;
	else if (downPressed() && tempVelocity.y > -0.6f)
		tempVelocity.y -= acceleration / 0.5f;
	else if (tempVelocity.y < 0)
		tempVelocity.y += acceleration / 0.5f;
	else if (tempVelocity.y > 0)
		tempVelocity.y -= acceleration / 0.5f;

	// Set plane sprite var before we reset velocity for bounds
	if (tempVelocity.y == 0)
		planeVertical = 0;
	else if (tempVelocity.y < 0)
		planeVertical = 1;
	else if (tempVelocity.y > 0)
		planeVertical = 2;

	if (getPos().y <= 69.6 && tempVelocity.y < 0)
		tempVelocity.y = 0;
	else if (getPos().y >= 139.4 && tempVelocity.y > 0)
		tempVelocity.y = 0;

	if (stage != 1)
		sizeIndex = 0;
	sprite->setTextureRect(playerTextures[planeVertical][sizeIndex]);

	// Spawn bullets
	if (zPressed() && bulletCD.getElapsedTime().asMilliseconds() >
		BULLET_COOLDOWN / gameSpeed)
	{
		bulletCD.restart();

		bullets.
			push_back(new CharacterBullet(spriteSheet, getPos(), sizeIndex));

		bulletSound.play();
	}
#ifndef NDEBUG
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		std::cout << getPos().x << " " << getPos().y << " " << getPos().z <<
			"\n";
#endif

	if (stage != 3)
	{
		if (stage != 1)
			tempVelocity.z = -1.3f; //for moving with background
		else // space moves 2/3 speed
			tempVelocity.z = -1.3f * 0.66f;
	}
	else
		tempVelocity.z = 0;

	// Position updates
	setVelocity(tempVelocity);
	shadow.setPosition(
		translateTo2d(sf::Vector3f(getPos().x - 5, 2 * 224 / 3, getPos().z)));
	hitmarker.setPosition(sprite->getPosition() + sf::Vector2f(40, -15));

	// Shadow before character
	if (stage != 1)
		window.draw(shadow);
	else if (hitmarkerTimer.getElapsedTime().asMilliseconds() < 150)
		window.draw(hitmarker);

	// if you stay above 80 for 3s and stage is initial then you get missile after you
	if (getPos().y > 80 || stage != 0)
		missileTimer.restart();

	// Drawing
	Character::update(window, gameSpeed);
	// updating position using velocity, draw character
	updateBullets(window, gameSpeed);

#ifndef NDEBUG
	debugText.setString(std::to_string(static_cast<int>(getPos().x)) + " " +
		std::to_string(static_cast<int>(getPos().y)) + " " + std::to_string(
			static_cast<int>(getPos().z))
	);
	debugText.setPosition(sprite->getPosition());
	window.draw(debugText);
#endif // !NDEBUG
}


/// <summary>
/// Modify variables for when player dies or respawns.
/// </summary>
void Player::kill()
{
	alive = !alive;
}


/// <summary>
/// Reset the players position to default.
/// </summary>
/// <param name="zOffset"></param>
void Player::resetPos(int zOffset)
{
	setPos(sf::Vector3f(getPos().x, getPos().y, zOffset * -1.33333f));
}


bool Player::isMissileable()
{
	return missileTimer.getElapsedTime().asSeconds() > 5 && getPos().z < -300;
}


/// <summary>
/// Check if player is alive
/// </summary>
/// <returns></returns>
bool Player::isAlive()
{
	return alive;
}
