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

struct PlayerData {
	byte lives = 2;
	int score = 0;
	int scoreThreshold = 10000;
};


// Clearer than using unsigned char often
using byte = unsigned char;


/// <summary>
/// The main game and all related classes eventually end up being used here.
/// Game loop is in here and is the 'heart' of the whole program.
/// </summary>
class Game
{
public:
	Game();
	~Game();
	void run();
private:
	sf::Texture spriteSheet, bossSheet, intro[5];
	Background* pBackground;
	sf::RenderWindow window;
	sf::Image icon;
	GUI gui;

	Player* player;
	std::vector<Enemy*> enemies;
	std::vector<Obstacle*> obstacles;
	std::vector<Wall*> walls;
	std::vector<ZapWall*> zapWalls;
	Boss* boss = nullptr;
	BossBullet* missile = nullptr;

	byte fuel = 128;
	byte completions = 0;
	byte lives = 2;
	PlayerData player1data, player2data;
	int highScore = 0;
	float gameSpeed = 1.2f;
	byte gameState = 0;
	int currentScores[6] = { 0, 0, 0, 0, 0, 0 };
	std::string currentNames[6] = { "   ", "   ", "   ", "   ", "   ", "   " };
	char name[3] = { '_', '_', '_' };
	bool activeCursor[4] = { true, true, true, true};
	byte selector = 0;
	std::fstream file;
	unsigned int reset = 0;
	unsigned int fps = 0;
	double deltaTime = 0;
	std::chrono::steady_clock::time_point lastTime =
		                                      std::chrono::high_resolution_clock::now()
	                                      , currentTime;
	bool player2 = false; // playing as player2
	bool player2mode = false;

	sf::View mainView;
	sf::View guiView;
	sf::SoundBuffer flightBuffer;
	sf::Sound flightSound;
	sf::Clock fuelClock;
	sf::Clock deathClock;
	sf::Sprite deathSprite, introLetters[6];
	sf::Clock playerScreenTimer;

	void doCollision(Player*);
	bool obstacleHit(Obstacle::ObstacleType type, sf::Vector3f difference,
	                 bool intersect2d);
	void playerDeath();
	void gameOver();
	void doIntro();
};