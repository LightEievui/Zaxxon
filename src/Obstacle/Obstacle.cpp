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


void Obstacle::create(sf::Vector3f pos, sf::Texture* tex, bool turret1, int dir)
{
	position = pos;

	turret = turret1;
	direction = dir;

	spriteSheet = tex;

	sprite.setTexture((*spriteSheet));
	sprite.setTextureRect(sf::IntRect(8, 112, 29, 19));

	sprite.setPosition(translateTo2d(pos));
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
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
