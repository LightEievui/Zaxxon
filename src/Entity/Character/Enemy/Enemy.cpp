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
			textures[i][j] = sf::IntRect(96 + 25*j + i*100, 37, 25, 25);
	this->sprite.setTextureRect(textures[0][0]);

	this->id = id;
	alive.restart();
	sf::Vector3f pos;
	// y range @ current values: 0 - 71.
	switch (id)
	{
	case 0:
		pos = sf::Vector3f(120, 25, spawnZ);
	}
	this->setPos(pos + sf::Vector3f(0, 69, 0));
}


/// <summary>
/// Run the logic for this enemy.
/// </summary>
/// <param name="window"></param>
void Enemy::update(sf::RenderWindow& window)
{	
	runAI();

	unsigned int planeSizeIndex = 0;
	unsigned int planeVertical = 0;
	getSizeIndex(planeSizeIndex);

	sf::Vector3f vel = getVelocity();
	if (vel.y > 0)
		planeVertical = 1;

	sprite.setTextureRect(textures[planeVertical][planeSizeIndex]);

	Character::update(window);
}


/// <summary>
/// Extra code for when enemy is killed.
/// </summary>
void Enemy::kill()
{

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
	}
}


/// <summary>
/// Basic enemy AI. Runs each frame on enemy update.
/// </summary>
void Enemy::runAI()
{
	sf::Vector3f vel;
	sf::Int32 msPassed = alive.getElapsedTime().asMilliseconds();
	double theta = 0;
	double angleScale = 1;

	switch (id)
	{
	case 0: // fish loop
		
		break;
	case 1:
		if (msPassed < 1000)
		{
			vel.y = 0.3;
		}
		break;
	}

	if (theta != 0)
	{
		sf::Vector2f transl = angleTranslate(sf::Vector2f(theta, angleScale));
		vel = sf::Vector3f(transl.x, vel.y, transl.y);
	}
	Character::setVelocity(vel);
}


sf::Vector2f Enemy::angleTranslate(sf::Vector2f angle)
{
	sf::Vector2f ret;
	ret.x = 1.4 * angle.y * cos(angle.x);
	ret.y = 1.2 * angle.y * sin(angle.x);

	return ret;
}