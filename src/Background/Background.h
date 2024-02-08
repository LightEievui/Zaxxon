#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>


class Background
{
public:
	Background();
	~Background();

	void create(std::string, sf::Vector2f = sf::Vector2f(0, 0));
	void setPosition(sf::Vector2f);
	void setTexture(std::string);
	void drawBackground(sf::RenderWindow&);
	//bool isEnd(sf::Vi);

private:
	sf::Texture texture;
	sf::Sprite back;
};

