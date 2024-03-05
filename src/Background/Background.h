#pragma once
#include <queue>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity/Character/Player/Player.h"
#include "Entity/Character/Enemy/Enemy.h"
#include "Obstacle/Obstacle.h"


class Background
{
public:
	enum Stage {
		INITIAL,
		SPACE,
		BOSS
	};

	Background(Stage, sf::View&, sf::Texture*, std::vector<Obstacle*>&, 
		std::vector<Enemy*>&, Player&, int);
	~Background();

	void update(sf::RenderWindow&, sf::View&, float, sf::Texture*, 
		std::vector<Obstacle*>&, std::vector<Enemy*>&, Player&);
	void setPosition(sf::Vector2f);

	// MAY want to consider changing to private later
	void generateObstacles(Background::Stage,
		std::vector<Obstacle*>&, sf::Texture*);
	void generateWaves(Background::Stage,
		std::vector<Enemy*>&, sf::Texture*, int);

	bool isInSpace(int z);
private:
	void changeStage(Stage, sf::View&, sf::Texture*,
		std::vector<Obstacle*>&, std::vector<Enemy*>&, Player&, int);
	bool backgroundFinished(sf::View&);
	void resetPos(sf::View&, Player&, int);

	sf::Texture initial, space, boss;
	sf::Sprite back;
	Stage stage = Stage::INITIAL;
	std::queue<std::pair<int, unsigned int>> waveQueue; // playerZ: id
};
