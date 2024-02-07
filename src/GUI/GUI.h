#include <vector>
#include "ZaxxonText/ZaxxonText.h"
#include "HMSection/HMSection.h"
#pragma once


class GUI
{
public:
	GUI(sf::Texture* spritesheet);
	void render(sf::RenderWindow& window, float playerY);
private:
	sf::Texture* spritesheet;

	sf::RectangleShape heightMeterBg;
	HMSection heightMeterSections[10];
	sf::Sprite heightH;
	sf::Sprite heightL;
	std::vector<sf::Transformable*> hudElements;
};

