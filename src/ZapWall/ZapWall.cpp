#include "ZapWall.h"


/// <summary>
/// ZapWall constructor.
/// </summary>
/// <param name="tex"></param>
/// <param name="pos"></param>
ZapWall::ZapWall(sf::Texture* tex, sf::Vector3f pos)
{
	spriteSheet = tex;
	startPosition = pos;
	setPos(pos);

	sprite->setTexture(*spriteSheet);
	sprite->setPosition(translateTo2d(sf::Vector3f(pos.x, pos.y, pos.z)));
	sprite->setTextureRect(sf::IntRect(199, 63, 35, 35));
}


/// <summary>
/// Update the ZapWall.
/// </summary>
/// <param name="window"></param>
/// <param name="gameSpeed"></param>
void ZapWall::update(sf::RenderWindow& window, float gameSpeed)
{
	//Only draws if on screen and has delay so it does not stop too early
	if (!getWindowViewRect(window).intersects(sprite->getGlobalBounds()))
	{
		if (drawn == true)
			offScreenCount++;

		if (offScreenCount == 15)
		{
			offScreenCount = 15;
			onScreen = false;
			drawn = false;
			return;
		}
	}

	onScreen = true;
	drawn = true;

	window.draw(*sprite);

	//Movement every two frames
	if (count == 2)
	{
		//If on screen, move it over
		if (getPos().x > -190)
		{
			sprite->move(translateTo2d(sf::Vector3f(-58, 0, 0)));
			setPos(sf::Vector3f(getPos().x - 58, getPos().y, getPos().z));
		}
		//else it is off screen, reset it to start position
		else
		{
			sprite->setPosition(translateTo2d(sf::Vector3f(startPosition)));
			setPos(startPosition);
		}
		count = 0;
	}
	else
		count++;
}


/// <summary>
/// Sets start pos for the ZapWall.
/// </summary>
void ZapWall::setStartPos(sf::Vector3f pos)
{
	startPosition = pos;
}


/// <summary>
/// 
/// </summary>
/// <returns>If the ZapWall is on screen</returns>
bool ZapWall::isOnScreen()
{
	return onScreen;
}


/// <summary>
/// 
/// </summary>
/// <returns>Start position</returns>
sf::Vector3f ZapWall::getStartPosition()
{
	return startPosition;
}