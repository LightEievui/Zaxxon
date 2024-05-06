#pragma once
#include <SFML/Graphics.hpp>


// Util contains helpful methods that are used in many places around the project.
sf::Vector2f translateTo2d(sf::Vector3f);
sf::FloatRect getWindowViewRect(sf::RenderWindow&);

static bool util_zPress = true;
bool zPressed();
bool upPressed();
bool downPressed();
bool leftPressed();
bool rightPressed();
sf::FloatRect getWindowViewRect(sf::RenderWindow&);
