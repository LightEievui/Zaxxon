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
			textures[i][j] = sf::IntRect(96 + 25*(i+j), 37, 25, 25);
	this->sprite.setTextureRect(textures[0][0]);

	this->id = id;
	alive.restart();
	// y range @ current values: 0 - 71.
	switch (id)
	{
	case 0:
		this->setPos(sf::Vector3f(0, 100, spawnZ));
	}
	this->setPos(getPos() + sf::Vector3f(0, 69, 0));
	
	if (getPos().y >= yMax)
	{
		sf::Vector3f temp = getPos();
		temp.y = yMax-1;
		setPos(temp);
	}
}


/// <summary>
/// Run the logic for this enemy.
/// </summary>
/// <param name="window"></param>
void Enemy::update(sf::RenderWindow& window)
{	
	runAI();

	unsigned int planeSizeIndex;
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
		enemies.push_back(new Enemy(spritesheet, 0, playerZ - 1000));
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

	switch (id)
	{
	case 0:
		if (msPassed%2000 < 1000)
		{
			vel.y = -0.3;
			vel.z = 0.3;
		}
		else
		{
			vel.y = 0.3;
			vel.z = -0.3;
		}
		break;
	}

	if ((getPos().x < xMin && vel.x < 0) || (getPos().x > xMax && vel.x > 0))
		vel.x = 0;
	else if ((getPos().y < yMin && vel.y < 0) || (getPos().y > yMax && vel.y > 0))
		vel.y = 0;

	Character::setVelocity(vel);
}