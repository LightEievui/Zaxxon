#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Util/Util.h"


/// <summary>
/// Class for the normal walls (stone brick).
/// </summary>
class Wall
{
public:
	Wall(sf::Texture*, sf::Vector3f, int size, std::vector<int>);

	void drawWalls(sf::RenderWindow& window);
	void setPosition(sf::Vector3f, int piece = 0);
	void setTexture(int, int);
	bool checkOnScreen();
	std::vector<sf::Vector3f>& getWallPositions();
private:
	sf::Texture* spriteSheet;
	bool onScreen = false;
	std::vector<sf::Sprite> sprites;
	std::vector<sf::Vector3f> wallPositions;
};