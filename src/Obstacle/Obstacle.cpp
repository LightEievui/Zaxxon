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


void Obstacle::create(sf::Vector3f pos, std::string file, bool turret1, int dir)
{
	position = pos;

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


sf::Vector3f Obstacle::getPosition()
{
	return position;
}


std::vector<sf::FloatRect> Obstacle::getBulletLocations()
{
	return bulletSprites.;
}


void Obstacle::update(sf::RenderWindow& window)
{
	sf::Vector2f wPos = sf::Vector2f(window.getView().getCenter().x - (window.getView().getSize().x / 2),
		window.getView().getCenter().y - (window.getView().getSize().y / 2));

	if (onScreen = sf::FloatRect(wPos.x, wPos.y, window.getView().getSize().x,
		window.getView().getSize().y).intersects(sprite.getGlobalBounds()))
	{
		if (turret)
		{
			if (count % 100 == 0)
			{
				sf::Sprite temp;

				temp.setTexture(bulletTexture);
				temp.setPosition(sprite.getPosition());
				temp.setScale(sprite.getScale());
				temp.setOrigin()
				
				bulletSprites.push_back(temp);
				bulletPositions.push_back(position);
			}

			for (int i = 0; i < bulletSprites.size(); i++)
			{
				bulletPositions.at(i).z += 3;
				bulletSprites.at(i).setPosition(translateTo2d(bulletPositions.at(i)));
				window.draw(bulletSprites.at(i));
			}
		}
		window.draw(sprite);
		count = (count + 1) % 100;
	}
}


void Obstacle::setPosition(sf::Vector3f pos)
{
	sprite.setPosition(translateTo2d(pos));
}



bool Obstacle::isPresent()
{
	return onScreen;
}