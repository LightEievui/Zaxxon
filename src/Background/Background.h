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

	Background(Stage startStage, sf::View& mainView, sf::Texture* spritesheet,
		std::vector<Obstacle*>& obstacles, std::vector<Enemy*>& enemies, Player& player,
		int startPos
	);
	~Background();

	void update(sf::RenderWindow& window, sf::View& mainView, float gameSpeed,
		sf::Texture* spritesheet,
		std::vector<Obstacle*>& obstacles, std::vector<Enemy*>& enemies,
		Player& player
	);
	void setPosition(sf::Vector2f);

	// MAY want to consider changing to private later
	void generateObstacles(Background::Stage stage,
		std::vector<Obstacle*>& obstacles, sf::Texture* spriteSheet);
	void generateWaves(Background::Stage stage,
		std::vector<Enemy*>& enemies, sf::Texture* spriteSheet, int playerZ
	);
	bool isInSpace(int z);
private:
	void changeStage(Stage stage, sf::View& mainView, sf::Texture* spritesheet,
		std::vector<Obstacle*>& obstacles, std::vector<Enemy*>& enemies,
		Player& player, int startPos
	);
	bool backgroundFinished(sf::View&);
	void resetPos(sf::View& mainView, Player& player, int startPos);

	sf::Texture initial, space, boss;
	sf::Sprite back;
	Stage stage = Stage::INITIAL;
	std::queue<std::pair<int, unsigned int>> waveQueue; // playerZ: id
};

