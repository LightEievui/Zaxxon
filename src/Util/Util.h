#pragma once
#include <SFML/Graphics.hpp>


sf::Vector2f translateTo2d(sf::Vector3f);
sf::FloatRect getWindowViewRect(sf::RenderWindow&);
extern sf::Font debugFont;