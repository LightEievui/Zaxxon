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

	// Individual loading for copyright text
	copyright[0] = ZaxxonText::get(spritesheet, 28);
	copyright[1] = ZaxxonText::get(spritesheet, 29);
	copyright[2] = ZaxxonText::get(spritesheet, 'S');
	copyright[3] = ZaxxonText::get(spritesheet, 'E');
	copyright[4] = ZaxxonText::get(spritesheet, 'G');
	copyright[5] = ZaxxonText::get(spritesheet, 'A');
	copyright[6] = ZaxxonText::get(spritesheet, '1');
	copyright[7] = ZaxxonText::get(spritesheet, '9');
	copyright[8] = ZaxxonText::get(spritesheet, '8');
	copyright[9] = ZaxxonText::get(spritesheet, '2');

	for (int i = 0; i < 10; i++)
	{
	int start = 124;
	if (i >= 6)
		start += 16;
	else if (i >= 2)
		start += 8;
	copyright[i].setPosition(start + 8 * i, 244);
	}
}


void GUI::render(sf::RenderWindow& window, float playerY)
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

	for(int i = 0; i < 10; i++)
		window.draw(copyright[i]);
}
