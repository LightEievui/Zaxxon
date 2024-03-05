#include <SFML/Graphics.hpp>
#pragma once


class ZaxxonText
{
public:
	static sf::Sprite get(sf::Texture*, char);
	static void string(sf::Texture*, std::string, sf::Sprite*);
};