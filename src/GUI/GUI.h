#pragma once
#include <vector>
// temp
#include <iostream>
#include "ZaxxonText/ZaxxonText.h"
#include "HMSection/HMSection.h"


class GUI
{
public:
	GUI(sf::Texture* spritesheet);
	void render(sf::RenderWindow& window, float playerY, int score);

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
	sf::Sprite livesBar[2];
	sf::Sprite enemyLeft[4];
};
