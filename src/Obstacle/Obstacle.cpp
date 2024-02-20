#include "Obstacle.h"


Obstacle::Obstacle() : Entity()
{
}


Obstacle::~Obstacle()
{
	delete spriteSheet;
}


void Obstacle::create(sf::Vector3f pos, sf::Texture* tex, float shootInterval, int dir)
{
	position = pos;
	turret = true;
	total = shootInterval;
	direction = dir;

	if (direction == 0)
	{
		spriteSheet = tex;

		sprite.setTexture((*spriteSheet));
		sprite.setTextureRect(sf::IntRect(8, 112, 29, 19));

		sprite.setPosition(translateTo2d(pos));
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	}
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
		if (count % total == 0 && direction == 0)
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
		else if (count % total == 0)
		{
			bulletPositions.push_back(position);

			bulletSprites.push_back(sf::Sprite());
			bulletSprites.at(0).setTexture(*spriteSheet);
			bulletSprites.at(0).setTextureRect(sf::IntRect(80, 69, 19, 30));
			bulletSprites.at(0).setOrigin(bulletSprites.at(0).getGlobalBounds().width / 2, 0);
			bulletSprites.at(0).setPosition(translateTo2d(position));
		}
	}

	for (int i = 0; i < bulletSprites.size(); i++)
	{
		if (direction == 0)

			bulletPositions.at(i).z += 3;
		else
			bulletPositions.at(i).y -= .5;
		
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
	return onScreen;
}


void Obstacle::bulletKill(int bullet)
{
	bulletSprites.erase(bulletSprites.begin() + bullet);
	bulletPositions.erase(bulletPositions.begin() + bullet);
}
