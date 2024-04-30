#include "Obstacle.h"


/// <summary>
/// Create a turret obstacle with shooting and direction.
/// </summary>
/// <param name="pos"></param>
/// <param name="tex"></param>
/// <param name="delay">(type 5 only)Delay in frames before a rocket begins shooting.</param>
/// <param name="dir"></param>
Obstacle::Obstacle(sf::Vector3f pos, sf::Texture* tex, int delay, int dir) : Entity()
{
	random = (rand() % 1000) + 200;
	this->scoreIndicator = rand() % 3 + 1;
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
	//Shooting Up | Red Shooting Up
	else if (dir == 2 || dir == 4)
	{
		spriteSheet = tex;

		//sprite->setPosition(position);
		setPos(pos + sf::Vector3f(-14,0,0));
		sprite->setTexture((*spriteSheet));
		sprite->setTextureRect(sf::IntRect(72, 69, 32, 30));
		sprite->setPosition(translateTo2d(pos));
		sprite->setOrigin(sf::Vector2f(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2));
		rocketZ = delay;

		rocketExplosionSprite.setTexture(*spriteSheet);
		rocketExplosionSprite.setTextureRect(sf::IntRect(119, 71, 38, 27));
		rocketExplosionSprite.setPosition(translateTo2d(pos) + sf::Vector2f(0, 0));
		rocketExplosionSprite.setOrigin(sf::Vector2f(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2));
	}
	//Right Green Turrents
	else if (dir == 3)
	{
		type += 2;
		sprite->setTexture((*spriteSheet));
		sprite->setTextureRect(sf::IntRect(320, 153, 33, 22));

		sprite->setPosition(translateTo2d(pos));
		sprite->setOrigin(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2);
	}
	
	if (dir == 4)
	{
		redRocket = true;
		this->direction = 2;
		this->type = 5;
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
        sprite->setTextureRect(sf::IntRect(87, 106, 30, 30));
    else if (type == 2)
        sprite->setTextureRect(sf::IntRect(129, 109, 24, 28));
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


/// <summary>
/// Clean up memory related to the obstacle class
/// </summary>
Obstacle::~Obstacle()
{
	
}

/// <summary>
/// Death animation.
/// </summary>
/// <param name="animation"></param>
void Obstacle::kill(Animation::Anim animation)
{
	if (type == 5)
		animation = Animation::ALT_DEATH;
	animations.run(sprite, Animation::RESET);
	animations.run(sprite, animation, isTurret() ? scoreIndicator : 0);
};


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
void Obstacle::update(sf::RenderWindow& window, int playerZ)
{
	//Checks if obstacle is on screen
	if (!getWindowViewRect(window).intersects(sprite->getGlobalBounds()) || animations.getState() == 1)
	{
		onScreen = false;
		return;
	}

	onScreen = true;
	bool rocketExplosion = playerZ <= -rocketZ, rocketFiring = playerZ <= -rocketZ - 50;

	//Shooting mechanics for bassic turrets
	if (turret == true && direction != 2)
	{
		//Creates bullets for grey cannons
		if (count % total == 0 && direction == 0 && animations.getState() == 0)
		{
			bullets.push_back(ObstacleBullet(getPos(), spriteSheet, AbstractBullet::BulletType::zBullet));

			total = (rand() % 250) + 75;
        }
		//Creates bullets for green cannons
        else if (count % total == 0 && (direction == 1 || direction == 3) && animations.getState() == 0)
        {
			bullets.push_back(ObstacleBullet(getPos(), spriteSheet, AbstractBullet::BulletType::xBullet));

			total = (rand() % 250) + 75;
		}
	}
	//Controls the firing of up shooting missiles which are delayed from shooting afer entering the screen
	else if (direction == 2 && (animations.getState() == 0 || animations.getState() == 6 || animations.getState() == 7))
	{
		if (rocketFiring)
		{
			//Is the animation of launch still starting if so reset it
			if (animations.getState() == 3)
				animations.run(sprite, Animation::RESET);

			//is it below its explode height
			if (getPos().y > 70.f)
			{
				setPos(sf::Vector3f(getPos().x, getPos().y - 0.5f, getPos().z));
				sprite->setPosition(translateTo2d(getPos()));
				if (animations.getState() == 0 || animations.getState() == 7)
					animations.run(sprite, Animation::ROCKET_FLICKER, redRocket);
			}
			else if (animations.getState() == 7)
				kill(Animation::ALT_DEATH);
		}
		else if (rocketExplosion &&
			rocketAnimation.getState() < 3
		)
			rocketAnimation.run(&rocketExplosionSprite, Animation::LAUNCH);
	}

	//Gives bullets their direction
    for (unsigned int i = 0; i < bullets.size(); i++)
    {
		bulletPositions.at(i) = bullets.at(i).getPos();
		bullets.at(i).update(window);
    }
	
	//Offset position on death
	if (animations.getState() == 4 && !moved)
	{
		moved = true;
		if(isTurret())
			setPos(getPos() + sf::Vector3f(0, 0, 0));
		else
			setPos(getPos() + sf::Vector3f(0, 15, 0));
		sprite->setPosition(translateTo2d(getPos()));
	}

    if (type != 5 && (type != 7 || getPosition().x < 15))
        window.draw(*sprite);
	if (type == 5 && rocketExplosion)
	{
		if(rocketFiring)
			window.draw(*sprite);
		if(rocketAnimation.getState() != 4)
			window.draw(rocketExplosionSprite);
	}

    count = (count + 1) % total;

	//Moves Blue Space Gas Cans
    if (type == 7)
    {
        setPos(sf::Vector3f(getPosition().x + 1.f, getPosition().y - 0.6f, getPosition().z));
        sprite->setPosition(translateTo2d(sf::Vector3f(getPos().x + 1.f, getPos().y - 0.6f, getPos().z)));
    }

#ifndef NDEBUG
	debugText.setString(std::to_string((int)getPos().x) + " " +
		std::to_string((int)getPos().y) + " " + std::to_string((int)getPos().z)
		+ " " + std::to_string(animations.getState())
	);
	debugText.setPosition(sprite->getPosition());
	if(drawDebugText)
		window.draw(debugText);
#endif // !NDEBUG

}


/// <summary>
/// Default Entity::update override.
/// </summary>
void Obstacle::update(sf::RenderWindow& window)
{
	Obstacle::update(window, 0);
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
	return animations.getState() != 1 && animations.getState() != 2 && animations.getState() != 4 && onScreen;
}

/// <summary>
/// Checks if obstacle is a turret.
/// </summary>
/// <returns>Boolean</returns>
bool Obstacle::isTurret()
{
	return getType() == 3 || getType() == 4
		|| getType() == 8;
}

/// <summary>
/// Get rid of obstacles bullet by index (For turrets).
/// </summary>
/// <param name="bullet"></param>
void Obstacle::bulletKill(int bullet)
{
	bullets.erase(bullets.begin() + (bullet));
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
	8 = green shooting right
    */

	return type;
}

/// <summary>
/// Gets the score to add when the obstacle is destroyed.
/// </summary>
/// <returns>score</returns>
int Obstacle::getScore()
{
	int score = 0;

	switch (type)
	{
	case 1:
		score += 300;
		break;
	case 2:
		score += 1000;
		break;
	case 5:
		score += 150;
		break;
	case 6:
		score += 100;
		break;
	case 7:
		score += 300;
		break;
	default:
		switch (this->scoreIndicator)
		{
		case 1:
			score += 200;
		case 2:
			score += 500;
		case 3:
			score += 1000;
			break;
		}
		break;
	}

	return score;
}