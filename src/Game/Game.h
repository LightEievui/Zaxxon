#pragma once
#include <vector>
#include "Background/Background.h"



class Game
{
public:
	Game();
	~Game();
	void run();

private:
	// Player player;
	// std::vector<Enemy> enemy;
	// std::vector<Obstacle> obstacles;
	Background background = Background("Image.png", 1.6f, sf::Vector2f(0, 500), sf::Vector2f(-.4, .2));
	int fuel;
	int completions;
	int lives;
	int score;
	float backgroundSpeed;
	// std::vector<Texture> guiText;
	// std::vector<Sprite> gui;
};

