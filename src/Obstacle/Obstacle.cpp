
#include "Obstacle.h"


Obstacle::Obstacle(sf::Vector3f pos, sf::Texture* tex, float,  int dir) : Entity()
{
	srand(time(NULL));
	random = (rand() % 1000) + 200;

	this->type = dir + 3;

	setPos(pos);
	turret = true;
	direction = dir;

	spriteSheet = tex;

	//Grey Turrets = 0
	if (dir == 0)
	{
		sprite.setTexture((*spriteSheet));
		sprite.setTextureRect(sf::IntRect(8, 112, 29, 19));

		sprite.setPosition(translateTo2d(pos));
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	}
	//Green Turrets = 1
	else if (dir == 1)
	{
		sprite.setTexture((*spriteSheet));
		sprite.setTextureRect(sf::IntRect(48, 117, 30, 17));

		sprite.setPosition(translateTo2d(pos));
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	}
	//Shooting Up 
	else if (dir == 2)
	{
		spriteSheet = tex;

		//sprite.setPosition(position);

		sprite.setTexture((*spriteSheet));
		sprite.setTextureRect(sf::IntRect(80, 69, 19, 30));
		//sprite.setOrigin(bulletSprites.at(0).getGlobalBounds().width / 2, 0);
		sprite.setPosition(translateTo2d(pos));
	}
}


Obstacle::Obstacle(sf::Vector3f pos, sf::Texture* tex, int type) : Entity()
{
	/*
	KEY
	1 = gas can
	2 = satellite
	*/
	this->type = type;

	direction = -1;
	setPos(pos);
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

	sprite.setPosition(translateTo2d(pos));
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
}


Obstacle::~Obstacle()
{
	
}


sf::Vector3f Obstacle::getPosition()
{
	return getPos();
}


std::vector<sf::Vector3f> Obstacle::getBulletLocations()
{
	return bulletPositions;
}


void Obstacle::update(sf::RenderWindow& window)
{
	if (!getWindowViewRect(window).intersects(sprite.getGlobalBounds()) || animations.getState() == 1)
	{
		onScreen = false;
		return;
	}

	onScreen = true;

	if (turret == true)
	{
		if (count % total == 0 && direction == 0)
		{
			sf::Sprite temp;
			temp.setTexture((*spriteSheet));
			temp.setTextureRect(sf::IntRect(160, 129, 12, 8));
			temp.setPosition(translateTo2d(getPos()));
			temp.setOrigin(sf::Vector2f(0, temp.getGlobalBounds().height));
				
			bulletSprites.push_back(temp);
			bulletPositions.push_back(getPos());
		}
	}
	if (direction == 2)
	{
		setPos(sf::Vector3f(getPos().x, getPos().y - .5, getPos().z));
		sprite.setPosition(translateTo2d(getPos()));
	}


	for (int i = 0; i < bulletSprites.size(); i++)
	{
		if (direction == 0)
		{
			bulletPositions.at(i).z += 3;
		}
		bulletSprites.at(i).setPosition(translateTo2d(bulletPositions.at(i)));

		window.draw(bulletSprites.at(i));
	}

	window.draw(sprite);
	
	count = (count + 1) % total;
}


void Obstacle::setPosition(sf::Vector3f pos)
{
	sprite.setPosition(translateTo2d(pos));
}


bool Obstacle::isPresent()
{
	return animations.getState() != 0 ? !(animations.getState() != 0) : onScreen;
}


void Obstacle::bulletKill(int bullet)
{
	bulletSprites.erase(bulletSprites.begin() + (bullet));
	bulletPositions.erase(bulletPositions.begin() + bullet);
}


int Obstacle::getType()
{
	/*
	KEY
	1 = gas can
	2 = satellite
	3 = grey cannon
	4 = green cannon
	5 = Shooting Up
	*/

	return type;
}