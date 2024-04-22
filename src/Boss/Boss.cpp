#include "Boss.h"


/// <summary>
/// Prepare the boss variables
/// </summary>
/// <param name="start"></param>
/// <param name="target"></param>
/// <param name="bossSheet"></param>
/// <param name="spriteSheet"></param>
Boss::Boss(sf::Vector3f start, Entity* target, sf::Texture* bossSheet, sf::Texture* spriteSheet) : Entity()
{
	setPos(start);

	sprite->setPosition(translateTo2d(start));
	sprite->setTexture(*bossSheet);
	sprite->setTextureRect(sf::IntRect(0, 0, 58, 75));
	sprite->setOrigin(sf::Vector2f(0, sprite->getGlobalBounds().height * (3/4.)));

	this->target = target;

	this->spriteSheet = *spriteSheet;
	
	movementInt.restart();

	srand(time(NULL));

	targetPoints[0] = sf::Vector3f(((rand() % 100) / -100.) * 100., 0, 0);
	srand(time(NULL)+1);
	targetPoints[1] = sf::Vector3f((((rand() % 100) / -100.) * 100.) + 20, 0, 0);
}


/// <summary>
/// Clean up memory related to the boss
/// </summary> 
Boss::~Boss() 
{
	if (missile != nullptr)
	{
		delete missile;
	}
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

		if (targetPoints[stages].x - getPos().x > 3)
			setPos(sf::Vector3f(getPos().x+3, getPos().y, getPos().z));

		if (targetPoints[stages].x - getPos().x < 3)
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
			missile = new BossBullet(getPos(), target, &spriteSheet);
			missile->damage(hits);
		}
	}

	if (movementInt.getElapsedTime().asMilliseconds()%25)
	{
		if (hitCount < 4)
		{
			hitCount++;
			setPos(sf::Vector3f(getPos().x + ((rand() % 100) / 50.) - 1, getPos().y + ((rand() % 100) / 50.) - 1, getPos().z));
		}
		else if (hitCount == 4)
		{
			hitCount++;
			sprite->setColor(sf::Color(255, 255, 255));
		}
	}

	sprite->setPosition(translateTo2d(getPos()));

	if (stages == 2)
	{
		if (missile->isAlive())
		{
			delete missile;
			stages = 0;
		}
		else
			missile->update(window);
	}

	targetPoints[2] = sf::Vector3f(getPos().x, 0, 0);

	window.draw(*sprite);
}


void Boss::hit()
{
	if (hitCount > 4 && invFrames.getElapsedTime().asMilliseconds() >= 500)
	{
		invFrames.restart();
		hitCount = 0;
		sprite->setColor(sf::Color(225, 100, 100));
		hits++;

		if (hits >= 10)
			stages = 3;
	}
}


BossBullet* Boss::getMissile()
{
	return missile;
}


bool Boss::missileCreated()
{
	return bulletCreated;
}