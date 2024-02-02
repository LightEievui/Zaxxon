#include "Obstacle.h"


Obstacle::Obstacle()
{

}


Obstacle::~Obstacle()
{

}


void Obstacle::createObstacle(sf::Vector3f pos, std::string file, float scale = 1., bool turret = false, sf::Vector2f vel)
{
	if (!obstacleTexture.loadFromFile("res/" + file))
		std::cout << "Obstacle image file failed to load\n";

	obstacleSprite.setTexture(obstacleTexture);
}