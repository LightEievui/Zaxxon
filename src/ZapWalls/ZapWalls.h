#pragma once
#include <SFML/Graphics.hpp>
#include "Util/Util.h"


/// <summary>
/// Class for other walls (blue lightning).
/// </summary>
class ZapWalls
{
public:
	ZapWalls(sf::Texture*, sf::Vector3f);
	~ZapWalls();

	void drawZapWalls(sf::RenderWindow&);
	bool checkOnScreen();
	sf::Vector3f getStartPosition();

private:
	sf::Texture* spriteSheet;
	bool onScreen = false;
	sf::Sprite sprite;
	sf::Vector3f position;
	sf::Vector3f startPosition;
	int count = 0;
	int offScreenCount = 0;
	bool drawn = false;
};

