#include "Enemy.h"


/// <summary>
/// Create new enemy.
/// </summary>
/// <param name="texture"></param>
/// <param name="id"></param>
/// <param name="spawnZ"></param>
Enemy::Enemy(sf::Texture* texture, unsigned int id, sf::Vector3f spawnPos,
	int randOffset) : Character(texture)
{
	for (unsigned int i = 0; i < 2; i++)
		for (unsigned int j = 0; j < 4; j++)
			textures[i][j] = sf::IntRect(96 + 25 * j + i * 100, 37, 25, 25);
	this->sprite->setTextureRect(textures[0][0]);
	this->id = id;
	this->randOffset = randOffset;
	int spawnZ = (int)spawnPos.z;
	alive.restart();
	sf::Vector3f pos;

	// y range @ current values: 0 - 71.
	switch (id)
	{
	case 0:
		pos = sf::Vector3f(120, 25, (float)spawnZ);
		break;
	case 1:
		pos = sf::Vector3f(-30, 45, (float)spawnZ);
		break;
	case 2:
		pos = sf::Vector3f(-60, 55, (float)spawnZ);
		break;
	case 3:
		pos = sf::Vector3f(-90, 65, (float)spawnZ);
		break;
	case 4:
		pos = sf::Vector3f(0, 71, (float)spawnZ);
		break;
	case 5:
		pos = sf::Vector3f(-210, 71, (float)spawnZ);
		break;
	case 6:
		pos = sf::Vector3f(-160, 45, (float)spawnZ);
		break;
	case 7:
		pos = spawnPos;
	}
	this->setPos(pos + sf::Vector3f(0, 69, 0));
}


/// <summary>
/// Run the logic for this enemy.
/// </summary>
/// <param name="window"></param>
void Enemy::update(sf::RenderWindow& window, float gameSpeed)
{
	
	// Find velocity
	sf::Vector2f vel = runAI();

	// Shooting
	if ((!dead && 
		ableToFire &&
		bulletCD.getElapsedTime().asMilliseconds() > 250
		&& rand() % 125 == 0)
#ifndef NDEBUG
		|| (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
#endif
	)
	{
		bulletCD.restart();

		bullets.push_back(new CharacterBullet(spriteSheet, getPos(), sizeIndex,
			CharacterBullet::Enemy, this->sprite->getPosition())
		);
	}

	unsigned int planeVertical = vel.y > 0;

	// Moved to keep up with back
	sprite->move(translateTo2d(sf::Vector3f(0, 0, -1.3f * gameSpeed * 2/3)));
	sprite->setTextureRect(textures[planeVertical][sizeIndex]);

	// 
	Character::updateBullets(window);
	if(!dead)
		window.draw(*sprite);
}


/// <summary>
/// Get public size index of this enemy
/// </summary>
/// <returns></returns>
unsigned int Enemy::getSizeIndex()
{
	return sizeIndex;
}


/// <summary>
/// Extra code for when enemy is killed.
/// </summary>
void Enemy::kill()
{
	this->dead = true;
}


/// <summary>
/// Enemies come in waves, this will spawn the specified enemy wave by int id.
/// </summary>
/// <param name="enemies"></param>
/// <param name="spritesheet"></param>
/// <param name="playerZ"></param>
/// <param name="wave"></param>
void Enemy::spawnWave(std::vector<Enemy*>& enemies, sf::Texture* spritesheet,
	int playerZ, unsigned int wave)
{
	switch (wave)
	{
	case 0:
		// first fish loop
		enemies.push_back(new Enemy(spritesheet, 0, sf::Vector3f(0, 0, playerZ - 190.f)));
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		// first right->charge (top right)
		enemies.push_back(new Enemy(spritesheet, wave, sf::Vector3f(0, 0, playerZ - 360.f), rand() % 600 - 300));
		break;
	case 5:
	case 6:
		// (bottom left)
		enemies.push_back(new Enemy(spritesheet, wave, sf::Vector3f(0, 0, playerZ - 190.f), rand() % 600 - 300));
		break;
	case 7: // 3 ememies from top right
		for(int i = 0; i < 3; i++)
			enemies.push_back(new Enemy(spritesheet, wave, sf::Vector3f(35.f - 70.f*i, 0, playerZ - 360.f), rand() % 600 - 300));
	}
}


/// <summary>
/// Controls movement of enemies. Runs each frame on enemy update. 
/// </summary>
sf::Vector2f Enemy::runAI()
{
	sf::Vector2f vel;
	sf::Int32 msPassed = alive.getElapsedTime().asMilliseconds();
	float theta = 0;
	float scale = 1;
	sf::Vector2f transl;

	// size index 0-3 controls size: 0 biggest, 3 smallest
	switch (id)
	{
	case 0: // fish loop
		scale = 1.7f;

		if (msPassed < 1500)
		{
			theta = -60;
			if (msPassed < 600)
				sizeIndex = 1;
			else if (msPassed < 1200)
				sizeIndex = 2;
			else
				sizeIndex = 3;
		}
		else if (msPassed < 2500)
		{
			theta = (msPassed - 1500) / 2800.f * 360;
			theta = theta * PI / 180.f;
			transl.x = cos(theta) * scale;
			transl.y = 1.25f * -sin(theta) * scale; // eliptical
			theta = 0;
		}
		else if (msPassed < 5000)
		{
			// add 4k for starting pos of circle
			theta = (msPassed - 3400 + 4500) / 7500.f * 360;
			if (msPassed < 3500)
				sizeIndex = 2;
			else
				sizeIndex = 3;
		}
		else
		{
			theta = 180 + 60;
		}
		break;
	case 1: // come from right then up little then charge
	case 2:
	case 3:
		if (msPassed < 1500)
		{
			sizeIndex = 3;
			theta = 180 + 30;
		}
		else if (msPassed < 2500)
		{
			theta = 90;
			if (msPassed < 1800)
				sizeIndex = 2;
			else if (msPassed < 2100)
				sizeIndex = 1;
			else if (msPassed < 2500)
				sizeIndex = 0;
		}
		else if (msPassed < 5500)
		{
			theta = 180 + 40;
			if (msPassed < 3200)
				sizeIndex = 0;
			else if (msPassed < 3900)
				sizeIndex = 1;
			else if (msPassed < 4500)
				sizeIndex = 2;
			else
				sizeIndex = 3;
		}
		else
		{
			theta = 180;
		}
		break;
	case 4: // come from top right then down little then charge
		if (msPassed < 750 + randOffset)
		{
			sizeIndex = 0;
			theta = 180 - 15;
		}
		else if (msPassed < 2500 + randOffset*2)
		{
			theta = -90;
			scale = 0.6f;
			if (msPassed < 1000)
				sizeIndex = 0;
			else if (msPassed < 1500)
				sizeIndex = 1;
			else if (msPassed < 2000)
				sizeIndex = 2;
			else if (msPassed < 2500)
				sizeIndex = 3;
		}
		else if (msPassed < 5500 + randOffset * 3)
		{
			theta = 180 + 10;
			sizeIndex = 3;
		}
		else
		{
			theta = 180;
		}
		scale = abs(randOffset / 300.f) + 0.5f;
		break;
	case 5: // bottom right to slight upward charge
		theta = 180 - abs(randOffset)/300.f*45;
		scale = abs(randOffset) / 150.f + 0.5f;
		sizeIndex = 2;
		break;
	case 6: // alternate fish loop (from right)
		if (msPassed < 1000)
		{
			theta = (-msPassed-2000) / 4000.f * 360 ; // 180-270
			scale = 2.f;
			sizeIndex = 2;
		}
		else if (msPassed < 1700)
		{
			theta = (-msPassed - 1700) / 1000.f * 360;
			sizeIndex = 1;
			scale = 1.6f;
		}
		else
		{
			theta = 180 + 45;
			scale = 1.7f;
			sizeIndex = 2;
		}
		break;
	case 7: // 3 enemies from top right
		theta = 210;
		scale = 1.3f;
		sizeIndex = 1;
		break;
	}

	if (theta != 0)
		transl = angleTranslate(theta, scale);

	sprite->move(transl.x, transl.y);
	return transl;
}


/// <summary>
/// Translates an angle and scale to cartesian coordinates using trig.
/// </summary>
/// <param name="angle">An angle in degrees.</param>
/// <param name="scale">The amount to scale the coordinates by.</param>
/// <returns>A sf::vector2f with respective x and y.</returns>
sf::Vector2f Enemy::angleTranslate(float angle, float scale)
{
	sf::Vector2f ret;
	angle = angle * PI / 180.f;
	ret.x = cos(angle) * scale;
	ret.y = -sin(angle) * scale;

	return ret;
}