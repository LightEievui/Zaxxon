#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>


class Background
{
public:
	Background(std::string, float scale, sf::Vector2f = sf::Vector2f(0, 0), sf::Vector2f = sf::Vector2f(0, 0));
	~Background();

	void setPosition(sf::Vector2f);
	void setTexture(std::string);
	void setVelocity(sf::Vector2f);
	void drawBackground(sf::RenderWindow&);
	void reset();

private:
	sf::Texture texture;
	sf::Sprite back;
	sf::Vector2f vel, resetPos;
};

