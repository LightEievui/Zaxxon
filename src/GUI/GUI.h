#pragma once
#include <vector>
// temp
#include <iostream>
#include "ZaxxonText/ZaxxonText.h"
#include "HMSection/HMSection.h"

typedef unsigned char byte;


class GUI
{
public:
	GUI(sf::Texture*);
	void render(sf::RenderWindow&, float, int, int, byte, byte);
	void startRender(sf::RenderWindow&, int);

private:
	sf::Texture* spritesheet;

	sf::RectangleShape heightMeterBg;
	HMSection heightMeterSections[10];
	sf::Sprite heightH;
	sf::Sprite heightL;
	std::vector<sf::Transformable*> hudElements;

	sf::Sprite copyright[10];
	sf::Sprite topScore[9];
	sf::Sprite curScore[9];
	sf::Sprite fuelText[6];
	sf::Sprite enemyText[10];

	sf::Sprite fuelBar[16];
	sf::Sprite livesBar;
	sf::Sprite enemyLeft[4];

	sf::Sprite pushPlayerText[25];

	sf::Clock scoreClock;
};