#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Background/Background.h"
#include "Obstacle/Obstacle.h"
#include "Entity/Entity.h"
#include "Entity/Character/Player/Player.h"
#include "Entity/Character/Enemy/Enemy.h"
#include "GUI/GUI.h"


class Game
{
public:
	Game();
	~Game();
	void run();

private:
	// Player player;
	// std::vector<Enemy> enemy;
	std::vector<Obstacle*> obstacles;
	Background background;
	int fuel = 128;
	int completions;
	int lives;
	int score = 0;
	float gameSpeed = 1.;
	// GUI gui;
	sf::View mainView;
	sf::View guiView;

	void doCollision(Player*);
	void generateObstacles(sf::Texture*);
	void generateWaves(std::vector<Enemy*>& enemies, sf::Texture* spritesheet, int playerZ);
};
