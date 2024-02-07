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
	// 69top 135bottom so 6 y levels per section
	HMSection& activeSection = heightMeterSections[(int) ((playerY - 69)/6.66)];
	// TODO: activeSection.progress((playerY - 69) % 6.66);
	for (HMSection& section : heightMeterSections)
		window.draw(section);
	window.draw(heightH);
	window.draw(heightL);


	// MUST be done last because otherwise they will be off by a margin
	for (sf::Transformable* element : hudElements)
		element->move(sf::Vector2f(.8, -.4));
}