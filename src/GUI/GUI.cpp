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

}


void GUI::render(sf::RenderWindow& window, float playerY)
{
	window.draw(heightMeterBg);
	/* 69top 135bottom 66 in between, multiply by below number to get 100
	 should be 12 sections each for line/open and 2 each for top/bottom so
	 yDiff should be 100 max (100 from 12*8 + 4) also round to make sure correct
	*/
	int yDiff = round(((playerY) - 69)*1.51515151515);
	if (yDiff < 0)
		yDiff = 0;
	int selection = (yDiff-12) / 12 + 1;
	// -12 because I messed up lowest point
	if (yDiff <= 98 && yDiff >= 12)
	{
		// fix to 0
		yDiff -= 12;
		// set selection again
		selection = yDiff / 12 + 1;
		HMSection& activeSection = heightMeterSections[selection];
		activeSection.progress(yDiff % 12);
		std::cout << "ydiff: " << yDiff << " ydiff%12: " << yDiff % 12 << "\n";
	}
	else if (yDiff < 12)
	{
		// top
		heightMeterSections[0].progress(yDiff%12);
	}
	else if (yDiff > 98)
	{
		// bottom
	}
	
	for (int i = 0; i < 10; i++)
	{
		HMSection& section = heightMeterSections[i];
		if (i < selection && i < 8 && i > 0)
			section.progress(11);
		else if (i > selection && i < 8 && i > 0)
			section.progress(0);

		window.draw(section);
	}
	window.draw(heightH);
	window.draw(heightL);


	// MUST be done last because otherwise they will be off by a margin
	for (sf::Transformable* element : hudElements)
		element->move(sf::Vector2f(.8, -.4));
}