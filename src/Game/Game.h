#pragma once
#include <vector>


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
	// Background background;
	int fuel;
	int completions;
	int lives;
	int score;
	float backgroundSpeed;
	// std::vector<Texture> guiText;
	// std::vector<Sprite> gui;
};

