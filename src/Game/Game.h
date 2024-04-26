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
#include "Entity/Boss/Boss.h"


typedef unsigned char byte;


class Game
{
public:
	Game();
	~Game();
	void run();

private:
	sf::Texture spriteSheet, bossSheet;
	Background* pBackground;
	sf::RenderWindow window;
	sf::ContextSettings s;
	GUI gui;

	Player* player;
	std::vector<Enemy*> enemies;
	std::vector<Obstacle*> obstacles;
	std::vector<Wall*> walls;
	std::vector<ZapWalls*> zapWalls;
	Boss* boss = nullptr; //file position needs to be moved inside of entity, fsr my home computer struggles to edit file position in visual studio

	byte fuel = 128;
	byte completions = 0;
	byte lives = 2;
	int score = 0;
	int highScore = 0;
	float gameSpeed = 1.0;
	byte gameState = 0;
	int currentScores[6] = { 0, 0, 0, 0, 0, 0 };
	std::string currentNames[6] = { "   ", "   ", "   ", "   ", "   ", "   " };
	char name[3] = { '_', '_', '_' };
	bool activeCursor[4] = { true, true, true, true};
	byte selector = 0;
	std::fstream file;
	unsigned int fps = 0;
	double deltaTime = 0;
	std::chrono::steady_clock::time_point lastTime =
		std::chrono::high_resolution_clock::now(), currentTime;

	sf::View mainView;
	sf::View guiView;
	sf::SoundBuffer flightBuffer;
	sf::Sound flightSound;
	sf::Clock fuelClock;
	sf::Clock deathClock;
	sf::Sprite deathSprite;

	bool bossState = true;

	void doCollision(Player*);
	void playerDeath();
	void gameOver();
};