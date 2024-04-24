#pragma once
#include <queue>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity/Character/Player/Player.h"
#include "Entity/Character/Enemy/Enemy.h"
#include "Obstacle/Obstacle.h"
#include "Wall/Wall.h"
#include "ZapWalls/ZapWalls.h"


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
		std::vector<Enemy*>&, Player&, int, std::vector<Wall*>&, std::vector <ZapWalls*>&);
	~Background();

	void update(sf::RenderWindow&, sf::View&, float, sf::Texture*,
		std::vector<Obstacle*>&, std::vector<Enemy*>&, Player&, std::vector<Wall*>&, bool, std::vector <ZapWalls*>&);
	void setPosition(sf::Vector2f);

	// MAY want to consider changing to private later
	void generateObstacles(Background::Stage,
		std::vector<Obstacle*>&, sf::Texture*, std::vector<Wall*>&, std::vector <ZapWalls*>&);
	void generateWaves(Background::Stage,
		std::vector<Enemy*>&, sf::Texture*, int);
	void resetPos(sf::View&, Player&, int);

	Stage getStage();
	void setStage(Stage);

	bool isInSpace(int);

	void flashColor(sf::RenderWindow&);

private:
	void changeStage(Stage, sf::View&, sf::Texture*,
		std::vector<Obstacle*>&, std::vector<Enemy*>&, Player&, int, std::vector<Wall*>&, std::vector <ZapWalls*>&);
	bool backgroundFinished(sf::View&);

	sf::Texture initial, space, boss, death;
	sf::Sprite back;
	sf::Sprite deathOverlay;
	Stage stage = Stage::INITIAL;
	std::queue<std::pair<int, unsigned int>> waveQueue; // playerZ: id
};