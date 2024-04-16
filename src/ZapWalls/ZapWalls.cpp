#include "ZapWalls.h"


//Sets up sprite
ZapWalls::ZapWalls(sf::Texture* tex, sf::Vector3f pos)
{
	spriteSheet = tex;
	startPosition = pos;
	position = pos;

	sprite.setTexture(*spriteSheet);
	sprite.setPosition(translateTo2d(sf::Vector3f(pos.x, pos.y, pos.z)));
	sprite.setTextureRect(sf::IntRect(199, 63, 35, 35));
}


ZapWalls::~ZapWalls()
{

}


//Moves the wall and draws
void ZapWalls::drawZapWalls(sf::RenderWindow& window)
{
	//Only draws if on screen and has delay so it does not stop too early
	if (!getWindowViewRect(window).intersects(sprite.getGlobalBounds()))
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

	window.draw(sprite);
	
	//Movement every two frames
	if (count == 2)
	{
		if (position.x > -200)
		{
			sprite.move(translateTo2d(sf::Vector3f(-58, 0, 0)));
			position = sf::Vector3f(position.x - 58, position.y, position.z);
		}
		else
		{
			sprite.setPosition(translateTo2d(sf::Vector3f(startPosition)));
			position = startPosition;
		}
		count = 0;
	}
	else
		count++;
}


bool ZapWalls::checkOnScreen()
{
	return onScreen;
}


//Returns start Position of moving walls for collisions
sf::Vector3f ZapWalls::getStartPosition()
{
	return startPosition;
}