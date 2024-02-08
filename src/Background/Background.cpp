#include "Background.h"


Background::Background()
{

}


Background::~Background()
{

}


void Background::drawBackground(sf::RenderWindow& window)
{
	window.draw(back);
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

void Background::create(std::string file, sf::Vector2f pos)
{
	if (!texture.loadFromFile("res/" + file))
		std::cout << "Background file could not load\n";

	back.setTexture(texture);
	//Sets the origin to the bottom left corner as that is where it will start 
	//on the screen
	back.setOrigin(sf::Vector2f(0, texture.getSize().y));
	back.setPosition(pos);
}


