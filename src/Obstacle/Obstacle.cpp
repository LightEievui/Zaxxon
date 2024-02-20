#include "Obstacle.h"


Obstacle::Obstacle(sf::Vector3f pos, sf::Texture* tex, float,  int dir) : Entity()
{
	srand(time(NULL));
	random = (rand() % 1000) + 200;


	position = pos;
	turret = true;
	direction = dir;

	spriteSheet = tex;

	sprite.setTexture((*spriteSheet));
	sprite.setTextureRect(sf::IntRect(8, 112, 29, 19));

	sprite.setPosition(translateTo2d(pos));
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
}


Obstacle::Obstacle(sf::Vector3f pos, sf::Texture* tex, int type) : Entity()
{
	/*
	KEY
	1 = gas can
	2 = satellite
	*/

	position = pos;
	turret = false;
	spriteSheet = tex;

	sprite.setTexture((*spriteSheet));

	if (type == 1)
	{
		sprite.setTextureRect(sf::IntRect(87, 106, 30, 30));
	}
	else if (type == 2)
	{
		sprite.setTextureRect(sf::IntRect(129, 109, 24, 28));
	}
	
		//GreenTurret::sprite.setTextureRect(sf::IntRect(48, 117, 30, 17));

	sprite.setPosition(translateTo2d(pos));
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
}


Obstacle::~Obstacle()
{
	
}


sf::Vector3f Obstacle::getPosition()
{
	return position;
}


std::vector<sf::Vector3f> Obstacle::getBulletLocations()
{
	return bulletPositions;
}


void Obstacle::update(sf::RenderWindow& window)
{
	if (!getWindowViewRect(window).intersects(sprite.getGlobalBounds()))
	{
		onScreen = false;
		return;
	}

	onScreen = true;

	if (turret)
	{
		if (count % 100 == 0)
		{
			sf::Sprite temp;
			temp.setTexture((*spriteSheet));
			temp.setTextureRect(sf::IntRect(160, 129, 12, 8));
			temp.setPosition(sprite.getPosition() + translateTo2d(sf::Vector3f(0, 0, 30)));
			temp.setScale(sprite.getScale());
			temp.setOrigin(sf::Vector2f(0, temp.getGlobalBounds().height));
				
			bulletSprites.push_back(temp);
			bulletPositions.push_back(position + sf::Vector3f(0, 0, 30));
		}
	}

	for (int i = 0; i < bulletSprites.size(); i++)
	{
		bulletPositions.at(i).z += 3;
		bulletSprites.at(i).setPosition(translateTo2d(bulletPositions.at(i)));
		window.draw(bulletSprites.at(i));
	}

	window.draw(sprite);
	count = (count + 1) % 100;
}


void Obstacle::setPosition(sf::Vector3f pos)
{
	sprite.setPosition(translateTo2d(pos));
}


bool Obstacle::isPresent()
{
	return onScreen;
}


void Obstacle::bulletKill(int bullet)
{
	bulletSprites.erase(bulletSprites.begin() + bullet);
	bulletPositions.erase(bulletPositions.begin() + bullet);
}
