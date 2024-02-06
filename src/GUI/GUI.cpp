#include "GUI.h"


GUI::GUI(sf::Texture* spritesheet)
{
	this->spritesheet = spritesheet;
	heightMeterBg.setFillColor(sf::Color::Black);
	heightMeterBg.setPosition(0, 73);
	heightMeterBg.setSize(sf::Vector2f(8, 89));

	heightH = ZaxxonText::get(spritesheet, 'H');
	heightH.setPosition(0, 73-8);
	heightH.setColor(sf::Color(222, 0, 0)); // red right of chars

	heightL = ZaxxonText::get(spritesheet, 'L');
	heightL.setPosition(0, 162);
	heightL.setColor(sf::Color(0, 222, 0)); // green right of chars

	for (int i = 0; i < 10; i++)
	{
		HMSection& section = heightMeterSections[i];
		section.setTexture(*spritesheet);
		if (i % 2 == 1) // odd
			section.setType(HMSection::OPEN);
		else
			section.setType(HMSection::LINE);
		section.setPosition(0, heightH.getPosition().y + 16 + 8*i);
	}
}


void GUI::render(sf::RenderWindow& window)
{
	// temp
	//window.setFramerateLimit(1);
	window.draw(heightMeterBg);
	for (HMSection& section : heightMeterSections)
	{
		//section.setSection(section.getSection() + 1);
		window.draw(section);
	}
	window.draw(heightH);
	window.draw(heightL);
}