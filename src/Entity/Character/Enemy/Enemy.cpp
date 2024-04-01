#include "Enemy.h"


/// <summary>
/// Create new enemy.
/// </summary>
/// <param name="texture"></param>
/// <param name="id"></param>
/// <param name="spawnZ"></param>
Enemy::Enemy(sf::Texture* texture, unsigned int id, int spawnZ) : Character(texture)
{
	for (unsigned int i = 0; i < 2; i++)
		for (unsigned int j = 0; j < 4; j++)
			textures[i][j] = sf::IntRect(96 + 25 * j + i * 100, 37, 25, 25);
	this->sprite->setTextureRect(textures[0][0]);
	this->id = id;
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
	}
	this->setPos(pos + sf::Vector3f(0, 69, 0));
}


/// <summary>
/// Run the logic for this enemy.
/// </summary>
/// <param name="window"></param>
void Enemy::update(sf::RenderWindow& window, float gameSpeed)
{
	sf::Vector2f vel = runAI();
	if ((ableToFire &&
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

	// keep up with back
	sprite->move(translateTo2d(sf::Vector3f(0, 0, -1.3f * gameSpeed)));
	sprite->setTextureRect(textures[planeVertical][sizeIndex]);
	for (CharacterBullet* bullet : bullets)
		bullet->update(window);


	window.draw(*sprite);
}


unsigned int Enemy::getSizeIndex()
{
	return sizeIndex;
}


/// <summary>
/// Extra code for when enemy is killed.
/// </summary>
void Enemy::kill()
{
	delete this;
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
		enemies.push_back(new Enemy(spritesheet, 0, playerZ - 190));
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		// first right->charge
		enemies.push_back(new Enemy(spritesheet, wave, playerZ - 360));
		break;
	}
}


/// <summary>
/// Basic enemy AI. Runs each frame on enemy update.
/// </summary>
sf::Vector2f Enemy::runAI()
{
	sf::Vector2f vel;
	sf::Int32 msPassed = alive.getElapsedTime().asMilliseconds();
	float theta = 0;
	float scale = 1;
	sf::Vector2f transl;

	switch (id)
	{
	case 0: // fish loop COMPLETE
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
			transl.y = 1.25f * -sin(theta) * scale;
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
		if (msPassed < 750)
		{
			sizeIndex = 0;
			theta = 180 - 15;
		}
		else if (msPassed < 2500)
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
		else if (msPassed < 5500)
		{
			theta = 180 + 10;
			sizeIndex = 3;
		}
		else
		{
			theta = 180;
		}
		break;
	}

	if (theta != 0)
		transl = angleTranslate(theta, scale);

	sprite->move(transl.x, transl.y);
	return transl;
}


sf::Vector2f Enemy::angleTranslate(float angle, float scale)
{
	sf::Vector2f ret;
	angle = angle * PI / 180.f;
	ret.x = cos(angle) * scale;
	ret.y = -sin(angle) * scale;

	return ret;
}