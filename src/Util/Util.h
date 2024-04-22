#pragma once
#include <SFML/Graphics.hpp>


sf::Vector2f translateTo2d(sf::Vector3f);
sf::FloatRect getWindowViewRect(sf::RenderWindow&);

static bool util_zPress = true;
bool zPressed();
bool upPressed();
bool downPressed();
bool leftPressed();
bool rightPressed();