#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>


class Background
{
public:
	enum Stage {
		INITIAL,
		SPACE,
		BOSS
	};

	Background(sf::Vector2f = sf::Vector2f(0, 0));
	~Background();

	void update(sf::RenderWindow&, sf::View& mainView, float gameSpeed);
	void setPosition(sf::Vector2f);

private:
	bool backgroundFinished(sf::View&);
	void resetPos(sf::View& mainView);
	sf::Texture initial, space, boss;
	sf::Sprite back;
	Stage stage = Stage::INITIAL;
};

