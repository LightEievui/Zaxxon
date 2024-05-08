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

	this->spriteSheet = spriteSheet;

	movementInt.restart();

	//defines random movement, goes to two seperate points
	targetXPoints[0] = (rand() % 100) * -1.f;
	targetXPoints[1] = (abs((rand() - 528) * 72) % 100) * -1.f;

	//timer for invincibility frames
	invFrames.restart();
}


/// <summary>
/// Clean up memory related to the boss
/// </summary> 
Boss::~Boss()
{
	if (missile != nullptr)
		delete missile;
}


/// <summary>
/// Run the logic for the boss each frame and then draw it to the screen
/// </summary>
/// <param name="window"></param>
void Boss::update(sf::RenderWindow& window, float gameSpeed)
{
	if (movementInt.getElapsedTime().asMilliseconds() >= 100)
	{
		//moves to the target position every 100 millisecond to mimic gittery 
		//movment of the game
		movementInt.restart();

		if (targetXPoints[stages] - getPos().x > 3)
			setPos(sf::Vector3f(getPos().x + 3, getPos().y, getPos().z));

		if (targetXPoints[stages] - getPos().x < 3)
			setPos(sf::Vector3f(getPos().x - 3, getPos().y, getPos().z));

		if (abs(getPos().z - target->getPos().z) < 250 && stages == 0)
			stages++;

		//if the boss is far enough away from the player continue moving it
		if (abs(getPos().z - target->getPos().z) > 150)
		{
			if (stages < 2)
				setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z + 3));
			else
				setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z - 7));
		}
		//else stop it and shoot the missile
		else
		{
			sprite->setTextureRect(sf::IntRect(58, 0, 58, 75));
			stages++;
			setPos(sf::Vector3f(getPos().x, getPos().y, getPos().z - 7));

			bulletCreated = true;
			//gives the missile the position of the player
			missile = new BossBullet(
				sf::Vector3f(getPos().x - 33, getPos().y - 19, getPos().z),
				target, spriteSheet);
			missile->damage(hits);
		}
	}

	//detects the hits to the missile and adds the shaking animation
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

	//if it retreats all the way it has been defeated
	if (getPos().z <= -4000 && stages >= 2)
	{
		destroyed = true;
		stages++;
	}

	targetXPoints[2] = getPos().x;

	window.draw(*sprite);

	//if the missile dies delete it and record that action
	if (stages == 2 && missile != nullptr)
	{
		if (missile->isHit())
		{
			delete missile;
			missile = nullptr;
		}
		else if (missile->collided())
			bulletCreated = false;
		else
			missile->update(window, gameSpeed);
	}
}


/// <summary>
/// Run visual effects and code for when you land a hit on the boss.
/// </summary>
void Boss::hit()
{
	//damages the boss and starts the timer of invincibility
	if (invFrames.getElapsedTime().asMilliseconds() >= 200)
	{
		invFrames.restart();
		hitCount = 0;
		sprite->setColor(sf::Color(225, 100, 100));
		hits++;
		if (hits >= 10)
		{
			sprite->setTextureRect(sf::IntRect(58, 0, 58, 75));
			stages = 3;
		}
	}
}


/// <summary>
/// Get the boss missile separately.
/// </summary>
/// <returns>The missile as a BossBullet*</returns>
BossBullet* Boss::getMissile() const
{
	return missile;
}


/// <summary>
/// Check if the boss missile exists.
/// </summary>
/// <returns>A boolean</returns>
bool Boss::missileCreated() const
{
	return bulletCreated;
}


/// <summary>
/// Check if the boss has been defeated.
/// </summary>
/// <returns>A boolean</returns>
bool Boss::isDestroyed() const
{
	return destroyed;
}
