#include <SFML/Graphics.hpp>
#pragma once


class ZaxxonText
{
public:
	static sf::Sprite get(sf::Texture* spritesheet, char in);
	static void string(sf::Texture* spritesheet, std::string in, sf::Sprite* arr);
private:
};
