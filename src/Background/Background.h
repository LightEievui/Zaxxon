#pragma once
#include <queue>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity/Character/Player/Player.h"
#include "Entity/Character/Enemy/Enemy.h"
#include "Obstacle/Obstacle.h"
#include "Wall/Wall.h"
#include "ZapWall/ZapWall.h"


/// <summary>
/// Holds all backgrounds and methods to fill each level with content.
/// </summary>
class Background
{
public:
	enum Stage
	{
		INITIAL = 0,
		SPACE = 1,
		BOSS = 2,
		BOSSFIGHT = 3,
		INTRO = 4
	};

	Background(Stage, sf::View&, sf::Texture*, std::vector<Obstacle*>&,
	           std::vector<Enemy*>&, Player&, int, std::vector<Wall*>&,
	           std::vector<ZapWall*>&);
	~Background();

	void update(sf::RenderWindow&, sf::View&, float, sf::Texture*,
	            std::vector<Obstacle*>&, std::vector<Enemy*>&, Player&,
	            std::vector<Wall*>&, std::vector<ZapWall*>&, unsigned int);
	void setPosition(sf::Vector2f);
	void generateObstacles(Stage,
	                       std::vector<Obstacle*>&, sf::Texture*,
	                       std::vector<Wall*>&, std::vector<ZapWall*>&);
	void generateWaves(Stage,
	                   std::vector<Enemy*>&, sf::Texture*, int);
	void resetPos(sf::View&, Player&, int);

	Stage getStage();
	void setStage(Stage);

	bool isInSpace(int);

	void flashColor(sf::RenderWindow&);

	void changeStage(Stage, sf::View&, sf::Texture*,
		std::vector<Obstacle*>&, std::vector<Enemy*>&, Player&,
		int, std::vector<Wall*>&, std::vector<ZapWall*>&);

private:
	bool backgroundFinished(sf::View&);

	sf::Texture initial, space, boss, death;
	sf::Sprite back;
	sf::Sprite deathOverlay;
	Stage stage = INITIAL;
	std::queue<std::pair<int, unsigned int>> waveQueue; // playerZ: id
};