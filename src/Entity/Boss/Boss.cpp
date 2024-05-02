#include "Boss.h"


/// <summary>
/// Prepare the boss variables
/// </summary>
/// <param name="start"></param>
/// <param name="target"></param>
/// <param name="bossSheet"></param>
/// <param name="spriteSheet"></param>
Boss::Boss(sf::Vector3f start, Entity* target, sf::Texture* bossSheet,
           sf::Texture* spriteSheet) : Entity()
{
	setPos(start);

	sprite->setPosition(translateTo2d(start));
	sprite->setTexture(*bossSheet);
	sprite->setTextureRect(sf::IntRect(0, 0, 58, 75));
	sprite->setOrigin(
		sf::Vector2f(0, sprite->getGlobalBounds().height * (3.f / 4.f)));

	this->target = target;

	this->spriteSheet = *spriteSheet;

	movementInt.restart();

	targetXPoints[0] = (rand() % 100) * -1.f;
	targetXPoints[1] = (abs((rand() - 528) * 72) % 100) * -1.f;

	invFrames.restart();
}


/// <summary>
/// Clean up memory related to the boss
/// </summary> 
Boss::~Boss()
{
	delete missile;
}


/// <summary>
/// Run the logic for the boss each frame and then draw it to the screen
/// </summary>
/// <param name="window"></param>
void Boss::update(sf::RenderWindow& window)
{
	if (movementInt.getElapsedTime().asMilliseconds() >= 100)
	{
		movementInt.restart();

		if (targetXPoints[stages] - getPos().x > 3)
			setPos(sf::Vector3f(getPos().x + 3, getPos().y, getPos().z));

		if (targetXPoints[stages] - getPos().x < 3)
			setPos(sf::Vector3f(getPos().x - 3, getPos().y, getPos().z));

		if (abs(getPos().z - target->getPos().z) < 250 && stages == 0)
			stages++;

		if (abs(getPos().z - target->getPos().z) > 150)
		{
			if (stages < 2)
				setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z + 3));
			else
				setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z - 7));
		}
		else
		{
			sprite->setTextureRect(sf::IntRect(58, 0, 58, 75));
			stages++;
			setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z - 7));

			bulletCreated = true;
			missile = new BossBullet(
				sf::Vector3f(getPos().x - 33, getPos().y - 19, getPos().z),
				target, &spriteSheet);
			missile->damage(hits);
		}
	}

	if (movementInt.getElapsedTime().asMilliseconds() % 25)
	{
		if (hitCount < 10)
		{
			hitCount++;
			setPos(sf::Vector3f(getPos().x + ((rand() % 100) / 50.f) - 1,
			                    getPos().y + ((rand() % 100) / 50.f) - 1,
			                    getPos().z));
		}
		else if (hitCount == 10)
		{
			hitCount++;
			sprite->setColor(sf::Color(255, 255, 255));
		}
	}

	sprite->setPosition(translateTo2d(getPos()));

	if (getPos().z <= -4000 && stages >= 2)
	{
		destroyed = true;
		stages++;
	}

	targetXPoints[2] = getPos().x;

	window.draw(*sprite);

	if (stages == 2 && missile != nullptr)
	{
		if (missile->isHit())
		{
			delete missile;
			missile = nullptr;
			bulletCreated = false;
		}
		else
			missile->update(window);
	}
}


/// <summary>
/// Run visual effects and code for when you land a hit on the boss.
/// </summary>
void Boss::hit()
{
	if (invFrames.getElapsedTime().asMilliseconds() >= 200)
	{
		invFrames.restart();
		hitCount = 0;
		sprite->setColor(sf::Color(225, 100, 100));
		hits++;
		if (hits >= 10)
			stages = 3;
	}
}


/// <summary>
/// Get the boss missile separately.
/// </summary>
/// <returns>The missile as a BossBullet*</returns>
BossBullet* Boss::getMissile()
{
	return missile;
}


/// <summary>
/// Check if the boss missile exists.
/// </summary>
/// <returns>A boolean</returns>
bool Boss::missileCreated()
{
	return bulletCreated;
}


/// <summary>
/// Check if the boss has been defeated.
/// </summary>
/// <returns>A boolean</returns>
bool Boss::isDestroyed()
{
	return destroyed;
}
