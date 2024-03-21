#pragma once
#include <vector>
#include <chrono>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Obstacle/Obstacle.h"
#include "Entity/Entity.h"
#include "Entity/Character/Player/Player.h"
#include "Entity/Character/Enemy/Enemy.h"
#include "GUI/GUI.h"
#include "Wall/Wall.h"
#include "Background/Background.h"


typedef unsigned char byte;


class Game
{
public:
	Game();
	~Game();
	void run();

private:
	sf::Texture spriteSheet;
	Background* pBackground;
	sf::RenderWindow window;
	sf::ContextSettings s;
	GUI gui;

	Player* player;
	std::vector<Enemy*> enemies;
	std::vector<Obstacle*> obstacles;
	std::vector<Wall*> walls;

	byte fuel = 128;
	byte completions = 0;
	byte lives = 2;
	int score = 0;
	int highScore = 0;
	float gameSpeed = 1.;
	byte gameState = 0;
	unsigned int highScores[6];
	std::fstream file;

	sf::View mainView;
	sf::View guiView;
	sf::SoundBuffer flightBuffer;
	sf::Sound flightSound;
	sf::Clock fuelClock;

	bool bossState = true;

	void doCollision(Player*);
	void playerDeath();
};