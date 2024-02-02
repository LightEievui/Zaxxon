#include "Background.h"


Background::Background()
{

}


Background::~Background()
{

}


void Background::drawBackground(sf::RenderWindow& window)
{
	//Moves the background based on the velocity;
	back.move(vel);

	window.draw(back);
}

void Background::reset()
{
	back.setPosition(resetPos);
}


void Background::setPosition(sf::Vector2f pos)
{
	back.setPosition(pos);
}


void Background::setTexture(std::string file)
{
	if (!texture.loadFromFile("res/" + file))
		std::cout << "Background file could not load -- setTexture()\n";

	back.setTexture(texture);
}


void Background::setSpeed(float speed)
{
	vel = sf::Vector2f(vel.x * speed, vel.y * speed);
}


void Background::setVelocity(sf::Vector2f velo)
{
	vel = velo;
}


void Background::create(std::string file, float scale, sf::Vector2f pos, sf::Vector2f velocity)
{
	vel = velocity;

	if (!texture.loadFromFile("res/" + file))
		std::cout << "Background file could not load\n";

	back.setTexture(texture);
	//Sets the origin to the bottom left corner as that is where it will start 
	//on the screen
	back.setOrigin(sf::Vector2f(0, texture.getSize().y));
	back.setPosition(pos);
	back.setScale(sf::Vector2f(scale, scale));

	//Base position to reset to.
	resetPos = pos;
}


bool Background::backgroundFinished()
{
	if (back.getPosition().x + back.getGlobalBounds().width < 0)
	{
		return true;
	}

	return false;
}