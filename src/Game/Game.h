#pragma once
#include <vector>
#include "Background/Background.h"
#include "Obstacle/Obstacle.h"
#include "Entity/Entity.h"
#include "Character/Player/Player.h"


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
	int fuel;
	int completions;
	int lives;
	int score = 0;
	float gameSpeed = 1.;
	// GUI gui;
	sf::View mainView;
	sf::View guiView;

	void doCollision(Player*);
	//sf::Vector3f playerPosConvertor(sf::Vector3f);
};
