#pragma once
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

	Background(sf::Vector2f = sf::Vector2f(0, 0));
	~Background();

	void update(sf::RenderWindow& window, sf::View& mainView, float gameSpeed,
		sf::Texture* spritesheet,
		std::vector<Obstacle*>& obstacles, std::vector<Enemy*>& enemies,
		Player& player
	);
	void setPosition(sf::Vector2f);

	// MAY want to consider changing to private later
	static void generateObstacles(Background::Stage stage,
		std::vector<Obstacle*>& obstacles, sf::Texture* spriteSheet);
	static void generateWaves(Background::Stage stage,
		std::vector<Enemy*>& enemies, sf::Texture* spriteSheet, int playerZ
	);
	bool isInSpace();
private:
	bool backgroundFinished(sf::View&);
	void resetPos(sf::View& mainView);
	

	sf::Texture initial, space, boss;
	sf::Sprite back;
	Stage stage = Stage::INITIAL;
};

