#pragma once
#include <vector>
#include "ZaxxonText/ZaxxonText.h"
#include "HMSection/HMSection.h"

using byte = unsigned char;


/// <summary>
/// Contains all GUI used in the game, including all text.
/// These are simple but can take a lot of space due to how much there is.
/// Different GUIs are split into different render functions.
/// </summary>
class GUI
{
public:
	GUI(sf::Texture*);
	~GUI();
	void render(sf::RenderWindow&, float, int, int, byte, byte);
	void startRender(sf::RenderWindow&, int);
	void renderScores(sf::RenderWindow&, int [], std::string []);
	void renderEnd(sf::RenderWindow&, byte = 255, byte = 0,
	               const char [3] = "   ");
	void renderWin(sf::RenderWindow&);

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

	sf::Sprite highScores[42];
	sf::Sprite highScoresText[10];
	sf::Sprite highScoresNames[18];

	sf::Sprite gameOver[8];
	sf::Sprite scoreEntry[14];
	sf::Sprite initialsText[12];
	sf::Sprite entryTime[13];
	sf::Sprite keyboard[30];
	sf::Sprite cursor;

	sf::Sprite endScreen[40];
};
