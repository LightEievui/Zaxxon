#pragma once
#include <queue>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity/Character/Player/Player.h"
#include "Entity/Character/Enemy/Enemy.h"
#include "Obstacle/Obstacle.h"
#include "Wall/Wall.h"


class Background
{
public:
	enum Stage {
		INITIAL = 0,
		SPACE = 1,
		BOSS = 2,
		BOSSFIGHT = 3
	};

	Background(Stage, sf::View&, sf::Texture*, std::vector<Obstacle*>&,
		std::vector<Enemy*>&, Player&, int, std::vector<Wall*>&);
	~Background();

	void update(sf::RenderWindow&, sf::View&, float, sf::Texture*,
		std::vector<Obstacle*>&, std::vector<Enemy*>&, Player&, std::vector<Wall*>&, bool);
	void setPosition(sf::Vector2f);

	// MAY want to consider changing to private later
	void generateObstacles(Background::Stage,
		std::vector<Obstacle*>&, sf::Texture*, std::vector<Wall*>&);
	void generateWaves(Background::Stage,
		std::vector<Enemy*>&, sf::Texture*, int);
	void resetPos(sf::View&, Player&, int);

	bool isInSpace(int z);
	int getStage();

private:
	void changeStage(Stage, sf::View&, sf::Texture*,
		std::vector<Obstacle*>&, std::vector<Enemy*>&, Player&, int, std::vector<Wall*>&);
	bool backgroundFinished(sf::View&);

	sf::Texture initial, space, boss;
	sf::Sprite back;
	Stage stage = Stage::INITIAL;
	std::queue<std::pair<int, unsigned int>> waveQueue; // playerZ: id
};