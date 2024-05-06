#include <SFML/Graphics.hpp>
#pragma once


/// <summary>
/// Class to grab text sprites from spritesheet given char or string.
/// </summary>
class ZaxxonText
{
public:
	static sf::Sprite get(sf::Texture*, char);
	static void string(sf::Texture*, std::string, sf::Sprite*);
};
