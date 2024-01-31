#include "Background.h"

Background::Background(std::string file, float scale, sf::Vector2f pos, sf::Vector2f velocity)
{
	vel = velocity;

	if (!texture.loadFromFile("res/Image.png"))
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