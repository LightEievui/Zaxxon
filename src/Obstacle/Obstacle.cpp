#include "Obstacle.h"


Obstacle::Obstacle()
{
	srand(time(NULL));
	random = (rand() % 1000) + 200;
}


Obstacle::~Obstacle()
{

}


void Obstacle::createObstacle(sf::Vector3f pos, std::string file, bool turret1, int dir)
{
	turret = turret1;
	direction = dir;

	if (!obstacleTexture.loadFromFile("res/" + file))
		std::cout << "Obstacle image file failed to load\n";
	if (dir == 0)
	{
		if (!bulletTexture.loadFromFile("res/RedBullet.png"))
		{
			std::cout << "Obstacle bullet image file failed to load\n";
		}
	}

	obstacleSprite.setTexture(obstacleTexture);

	obstacleSprite.setPosition(translateTo2d(pos));
}


void Obstacle::drawObstacle(sf::RenderWindow& window)
{
	if (turret)
	{
		if (count % 100 == 0)
		{
			sf::Sprite temp;

			temp.setTexture(bulletTexture);
			temp.setPosition(obstacleSprite.getPosition());
			temp.setScale(obstacleSprite.getScale());

			bulletSprites.push_back(temp);
		}

		sf::Vector2f wPos = sf::Vector2f(window.getView().getCenter().x - (window.getView().getSize().x / 2),
			window.getView().getCenter().y - (window.getView().getSize().y / 2));

		for (int i = 0; i < bulletSprites.size(); i++)
		{
			bulletSprites.at(i).move(sf::Vector2f(-0.8 * 2.5, 0.4 * 2.5));
			window.draw(bulletSprites.at(i));

			if (!sf::FloatRect(wPos.x, wPos.y, window.getView().getSize().x,
				window.getView().getSize().y).intersects(bulletSprites.at(i).getGlobalBounds()))
			{
				bulletSprites.erase(bulletSprites.begin() + i);
				std::cout << "erase";
			}
		}
	}
	window.draw(obstacleSprite);
	count = (count + 1) % 100;
}


void Obstacle::setPosition(sf::Vector3f pos)
{
	obstacleSprite.setPosition(translateTo2d(pos));
}
