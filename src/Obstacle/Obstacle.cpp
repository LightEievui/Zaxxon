#include "Obstacle.h"


Obstacle::Obstacle()
{
	srand(time(NULL));
	random = (rand() % 1000) + 200;
}


Obstacle::~Obstacle()
{

}


void Obstacle::createObstacle(sf::Vector3f pos, std::string file, float scale, sf::Vector2f vel, bool turret1, int dir)
{
	velocity = vel;
	turret = turret1;
	direction = dir;

	if (!obstacleTexture.loadFromFile("res/" + file))
		std::cout << "Obstacle image file failed to load\n";
	if (dir == 0)
	{
		if (!bulletTexture.loadFromFile("res/ZaxxonFull.png"))
		{
			std::cout << "Obstacle bullet image file failed to load\n";
		}
	}

	obstacleSprite.setTexture(obstacleTexture);

	obstacleSprite.setPosition(translateTo2d(pos));
	obstacleSprite.setScale(scale, scale);
}


void Obstacle::drawObstacle(sf::RenderWindow& window)
{
	count = (count + 1) % 10000;

	if (turret)
	{
		if (count % random == 0)
		{
			sf::Sprite temp;

			temp.setTexture(bulletTexture);
			temp.setPosition(obstacleSprite.getPosition());
			temp.setScale(obstacleSprite.getScale());

			bulletSprites.push_back(temp);
		}

		for (int i = 0; i < bulletSprites.size(); i++)
		{
			bulletSprites.at(i).move(sf::Vector2f(velocity.x * 3, velocity.y * 3));
		}
	}

	obstacleSprite.move(velocity);

	window.draw(obstacleSprite);
}


void Obstacle::setPosition(sf::Vector3f pos)
{
	obstacleSprite.setPosition(translateTo2d(pos));
}