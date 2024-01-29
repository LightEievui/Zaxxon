#include "Background.h"


Background::Background(std::string file, sf::Vector2f pos, sf::Vector2f velocity)
{
	vel = velocity;

	if (!texture.loadFromFile(file))
		std::cout << "Background file could not load\n";
	
	back.setTexture(texture);
	back.setPosition(pos);
}