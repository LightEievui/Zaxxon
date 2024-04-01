#include "Obstacle.h"


/// <summary>
/// Create a turret obstacle with shooting and direction.
/// </summary>
/// <param name="pos"></param>
/// <param name="tex"></param>
/// <param name=""></param>
/// <param name="dir"></param>
Obstacle::Obstacle(sf::Vector3f pos, sf::Texture* tex, float delay, int dir) : Entity()
{
	srand(time((time_t*)NULL));
	random = (rand() % 1000) + 200;

	this->type = dir + 3;

	setPos(pos);
	turret = true;
	direction = dir;

	spriteSheet = tex;

	//Grey Turrets = 0
	if (dir == 0)
	{
		sprite->setTexture((*spriteSheet));
		sprite->setTextureRect(sf::IntRect(8, 112, 29, 19));

		sprite->setPosition(translateTo2d(pos));
		sprite->setOrigin(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2);
	}
	//Green Turrets = 1
	else if (dir == 1)
	{
		sprite->setTexture((*spriteSheet));
		sprite->setTextureRect(sf::IntRect(48, 117, 30, 17));

		sprite->setPosition(translateTo2d(pos));
		sprite->setOrigin(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2);
	}
	//Shooting Up 
	else if (dir == 2)
	{
		spriteSheet = tex;

		//sprite->setPosition(position);

		sprite->setTexture((*spriteSheet));
		sprite->setTextureRect(sf::IntRect(72, 69, 32, 30));
		sprite->setPosition(translateTo2d(pos));
		sprite->setOrigin(sf::Vector2f(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2));
		total = delay;
	}
	//Right Green Turrents
	else if (dir == 3)
	{
		sprite->setTexture((*spriteSheet));
		sprite->setTextureRect(sf::IntRect(320, 153, 33, 22));

		sprite->setPosition(translateTo2d(pos));
		sprite->setOrigin(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2);
	}
}


/// <summary>
/// Create a non-turret obstacle (Still obstacle).
/// </summary>
/// <param name="pos"></param>
/// <param name="tex"></param>
/// <param name="type"></param>
Obstacle::Obstacle(sf::Vector3f pos, sf::Texture* tex, int type) : Entity()
{
    /*
    KEY
    1 = gas can
    2 = satellite
    3 = plane
    4 = blue space gas can
    */
    this->type = type;

	direction = -1;
	setPos(pos);
	turret = false;
	spriteSheet = tex;

	sprite->setTexture((*spriteSheet));

    if (type == 1)
    {
        sprite->setTextureRect(sf::IntRect(87, 106, 30, 30));
    }
    else if (type == 2)
    {
        sprite->setTextureRect(sf::IntRect(129, 109, 24, 28));
    }
    else if (type == 3)
    {
        this->type = 6;
        sprite->setTextureRect(sf::IntRect(92, 35, 29, 25));
    }
    else if (type == 4)
    {
        this->type = 7;
        sprite->setTextureRect(sf::IntRect(8, 195, 26, 29));
    }
    sprite->setPosition(translateTo2d(pos));
    sprite->setOrigin(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2);
}


Obstacle::~Obstacle()
{

}


/// <summary>
/// Get position of the obstacle.
/// </summary>
/// <returns>Vector of 3 floats</returns>
sf::Vector3f Obstacle::getPosition()
{
	return getPos();
}


/// <summary>
/// Get positions for all bullets of the obstacle (For turrets).
/// </summary>
/// <returns>Vector of vectors of 3 floats</returns>
std::vector<sf::Vector3f> Obstacle::getBulletLocations()
{
	return bulletPositions;
}


/// <summary>
/// Run the logic for this obstacle.
/// </summary>
/// <param name="window"></param>
void Obstacle::update(sf::RenderWindow& window)
{
	if (!getWindowViewRect(window).intersects(sprite->getGlobalBounds()) || animations.getState() == 1)
	{
		onScreen = false;
		return;
	}

	onScreen = true;

	if (turret == true && direction != 2)
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
		else if (count % total == 0 && direction == 1)
		{
			sf::Sprite temp;

			temp.setTexture((*spriteSheet));
			temp.setTextureRect(sf::IntRect(345, 124, 12, 8));
			temp.setPosition(translateTo2d(getPos()));

			bulletSprites.push_back(temp);
			bulletPositions.push_back(getPos());
		}
	}
	else if (direction == 2 && (animations.getState() == 0 || animations.getState() == 3))
	{
		if (count >= total)
		{
			if (animations.getState() == 3)
				animations.run(sprite, Animation::RESET);

			if (getPos().y > 70.f)
			{
				setPos(sf::Vector3f(getPos().x, getPos().y - .5f, getPos().z));
				sprite->setPosition(translateTo2d(getPos()));
			}
			else if (animations.getState() == 0)
			{
				kill(Animation::ALT_DEATH);
			}
		}
		else if (count < total && animations.getState() != 3)
		{
			std::cout << "on";
			animations.run(sprite, Animation::LAUNCH);
		}
	}


	for (unsigned int i = 0; i < bulletSprites.size(); i++)
	{
		if (direction == 0)
		{
			bulletPositions.at(i).z += 3;
		}
		else if (direction == 1)
		{
			bulletPositions.at(i).x += 3;
		}
		bulletSprites.at(i).setPosition(translateTo2d(bulletPositions.at(i)));

        window.draw(bulletSprites.at(i));
    }
 
    if (type != 7 || type == 7 && getPosition().x < 15)
        window.draw(*sprite);
 
    if (direction != 2)
        count = (count + 1) % total;
    else
        count = (count + 1) % 10000;

    if (type == 7)
    {
        //std::cout << "YES";
        setPos(sf::Vector3f(getPosition().x + 1, getPosition().y - 0.6, getPosition().z));
        sprite->setPosition(translateTo2d(sf::Vector3f(getPos().x + 1, getPos().y - 0.6, getPos().z)));
    }
}


/// <summary>
/// Set the position of this obstacle.
/// </summary>
/// <param name="pos"></param>
void Obstacle::setPosition(sf::Vector3f pos)
{
	sprite->setPosition(translateTo2d(pos));
}


/// <summary>
/// Check if this obstacle exists.
/// </summary>
/// <returns>A boolean</returns>
bool Obstacle::isPresent()
{
	return (animations.getState() == 0 || animations.getState() == 3) && onScreen == true ? true : false;
}


/// <summary>
/// Get rid of obstacles bullet by index (For turrets).
/// </summary>
/// <param name="bullet"></param>
void Obstacle::bulletKill(int bullet)
{
	bulletSprites.erase(bulletSprites.begin() + (bullet));
	bulletPositions.erase(bulletPositions.begin() + bullet);
}


/// <summary>
/// Get what type of obstacle this is.
/// </summary>
/// <returns>An int</returns>
int Obstacle::getType()
{
    /*
    KEY
    1 = gas can
    2 = satellite
    3 = grey cannon
    4 = green cannon
    5 = Shooting Up
    6 = Plane
    7 = blue floating gas can
    */

	return type;
}