#include "Obstacle.h"


Obstacle::Obstacle() : Entity()
{
	srand(time(NULL));
	random = (rand() % 1000) + 200;
}


Obstacle::~Obstacle()
{
	delete spriteSheet;
}


void Obstacle::createObstacle(sf::Vector3f pos, std::string file, bool turret1, int dir)
{
	turret = turret1;
	direction = dir;

	spriteSheet = new sf::Texture;

	if ((*spriteSheet).loadFromFile("res/" + file) == false)
		std::cout << "Obstacle image file failed to load\n";
	if (dir == 0)
	{
		if (!bulletTexture.loadFromFile("res/RedBullet.png"))
		{
			std::cout << "Obstacle bullet image file failed to load\n";
		}
	}

	sprite.setTexture((*spriteSheet));

	sprite.setPosition(translateTo2d(pos));
}


void Obstacle::update(sf::RenderWindow& window)
{
	if (!getWindowViewRect(window).intersects(sprite.getGlobalBounds()))
		return;
	std::cout << "draw";
	if (turret)
	{
		if (count % 100 == 0)
		{
			sf::Sprite temp;

			temp.setTexture(bulletTexture);
			temp.setPosition(sprite.getPosition());
			temp.setScale(sprite.getScale());

			bulletSprites.push_back(temp);
		}

		for (int i = 0; i < bulletSprites.size(); i++)
		{
			bulletSprites.at(i).move(sf::Vector2f(-0.8 * 2.5, 0.4 * 2.5));
			window.draw(bulletSprites.at(i));
		}
	}
	window.draw(sprite);
	count = (count + 1) % 100;
}


void Obstacle::setPosition(sf::Vector3f pos)
{
	sprite.setPosition(translateTo2d(pos));
}
