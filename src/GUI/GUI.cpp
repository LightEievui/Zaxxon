#include "GUI.h"


GUI::GUI(sf::Texture* spritesheet)
{
	this->spritesheet = spritesheet;
	heightMeterBg.setFillColor(sf::Color::Black);
	heightMeterBg.setPosition(0, 73);
	heightMeterBg.setSize(sf::Vector2f(8, 89));
	hudElements.push_back(&heightMeterBg);

	heightH = ZaxxonText::get(spritesheet, 'H');
	heightH.setPosition(0, 73-8);
	heightH.setColor(sf::Color(222, 0, 0)); // red right of chars
	hudElements.push_back(&heightH);

	heightL = ZaxxonText::get(spritesheet, 'L');
	heightL.setPosition(0, 162);
	heightL.setColor(sf::Color(0, 222, 0)); // green right of chars
	hudElements.push_back(&heightL);

	for (int i = 0; i < 10; i++)
	{
		HMSection& section = heightMeterSections[i];
		section.setTexture(*spritesheet);

		if (i == 0)
			section.setType(HMSection::TOP);
		else if (i == 9)
			section.setType(HMSection::BOTTOM);
		else if (i % 2 == 1) // odd number
			section.setType(HMSection::OPEN);
		else
			section.setType(HMSection::LINE);

		section.setPosition(0, heightH.getPosition().y + 16 + 8*i);
		hudElements.push_back(&section);
	}


	// Copyright text
	ZaxxonText::string(spritesheet, "\x1C\x1DSEGA1982", copyright);

	for (int i = 0; i < 10; i++)
	{
		copyright[i].setColor(sf::Color(222, 222, 247));

		int start = 124;
		if (i >= 6)
			start += 16;
		else if (i >= 2)
			start += 8;

		copyright[i].setPosition(start + 8 * i, 248);
	}


	// Top score text
	ZaxxonText::string(spritesheet, "TOP000000", topScore);

	for (int i = 0; i < 9; i++)
	{
		topScore[i].setColor(sf::Color(0, 222, 247));

		int start = 0;
		if (i > 2)
			start += 8;

		topScore[i].setPosition(start + 8 * i, 8);
	}


	// 1up score text
	ZaxxonText::string(spritesheet, "1UP000000", curScore);

	for (int i = 0; i < 9; i++)
	{
		curScore[i].setColor(sf::Color(222, 222, 247));

		int start = 0;
		if (i > 2)
			start += 8;

		curScore[i].setPosition(start + 8 * i, 24);
	}


	// Fuel text
	ZaxxonText::string(spritesheet, "FUELEF", fuelText);

	for (int i = 0; i < 6; i++)
	{
		if (i < 4)
		{
			fuelText[i].setColor(sf::Color(0, 222, 0));
			fuelText[i].setPosition(12 + i * 8, 240);
		}
		else
		{
			fuelText[i].setColor(sf::Color(222, 0, 0));
			fuelText[i].setPosition(52 + (i - 4) * 136, 240);
		}
	}


	// Enemy plane text
	ZaxxonText::string(spritesheet, "ENEMYPLANE", enemyText);

	for (int i = 0; i < 10; i++)
	{
		enemyText[i].setColor(sf::Color(222, 222, 247));
		enemyText[i].setPosition(176 + i % 5 * 8, 200 + i / 5 * 8);
	}


	// Fuel bar sprites
	for (int i = 0; i < 16; i++)
	{
		fuelBar[i].setTexture(*spritesheet);
		fuelBar[i].setTextureRect(sf::IntRect(328, 296, 8, 8));
		fuelBar[i].setPosition(60 + i * 8, 240);
	}


	// Lives bar sprites
	for (int i = 0; i < 2; i++)
	{
		livesBar[i].setTexture(*spritesheet);
		livesBar[i].setTextureRect(sf::IntRect(344, 288, 16, 16));
		livesBar[i].setPosition(12 + i * 16, 224);
	}

	
	// Enemies left
	ZaxxonText::string(spritesheet, "X\u000920", enemyLeft);
	enemyLeft[0].setTextureRect(sf::IntRect(368, 296, 8, 8));

	for (int i = 0; i < 4; i++)
		enemyLeft[i].setPosition(176 + i * 8, 216);
}


void GUI::render(sf::RenderWindow& window, float playerY, int score)
{
	window.draw(heightMeterBg);
	/* 69top 135bottom 66 in between, multiply by below number to get 68
	 should be 8 sections each for line/open and 2 each for top/bottom so
	 yDiff should be 68 max (68 from 8*8 + 4) also round to make sure correct
	*/
	int yDiff = round(((playerY) - 69)*1.0303030303);
	if (yDiff < 0)
		yDiff = 0;

	// -4 to make 64
	int selection = (yDiff-2) / 8;
	if (selection < 0)
		selection = 0;

	bool changingBelow = false, changingAbove = false;
	const int last = 7, localYDiff = (yDiff - 2) % 8;

	if (yDiff <= 66 && yDiff >= 2)
	{
		HMSection& activeSection = heightMeterSections[selection];

		activeSection.progress(localYDiff);

		if (localYDiff >= 5)
		{
			heightMeterSections[selection + 1].progress(localYDiff-7);
			changingBelow = true;
		}
	}
	else if (yDiff < 8)
	{
		// top
		heightMeterSections[0].progress(12+localYDiff);
	}
	else if (yDiff > 64)
	{
		// bottom
	}
	// temp
	//std::cout << "ydiff: " << yDiff << " localYDiff: " << localYDiff << "\n";
	
	for (int i = 0; i < 10; i++)
	{
		HMSection& section = heightMeterSections[i];
		if (i < selection)
		{
			if (i != selection - 1 || !changingAbove)
				section.empty();
		}
		else if (i > selection)
		{
			if(i != selection+1 || !changingBelow)
				section.fill();
		}

		window.draw(section);
	}
	window.draw(heightH);
	window.draw(heightL);

	// Update score
	for (int i = 0; i < 6; i++)
	{
		std::string str = std::to_string(score);

		while (str.length() < 6)
			str = '0' + str;

		curScore[8 - i] = ZaxxonText::get(spritesheet, str.at(5 - i));
		curScore[8 - i].setPosition(72 - 8 * i, 24);
	}

	for (int i = 0; i < 10; i++)
	{
		window.draw(copyright[i]);
		window.draw(enemyText[i]);
	}

	for (int i = 0; i < 9; i++)
	{
		window.draw(topScore[i]);
		window.draw(curScore[i]);
	}

	for (int i = 0; i < 6; i++)
		window.draw(fuelText[i]);

	for (int i = 0; i < 16; i++)
		window.draw(fuelBar[i]);

	for (int i = 0; i < 2; i++)
		window.draw(livesBar[i]);

	for (int i = 0; i < 4; i++)
		window.draw(enemyLeft[i]);
}
