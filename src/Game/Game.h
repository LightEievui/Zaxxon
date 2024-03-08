#pragma once
#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Background/Background.h"
#include "Obstacle/Obstacle.h"
#include "Entity/Entity.h"
#include "Entity/Character/Player/Player.h"
#include "Entity/Character/Enemy/Enemy.h"
#include "GUI/GUI.h"

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

	byte fuel = 128;
	byte completions = 0;
	byte lives = 3;
	int score = 0;
	float gameSpeed = 1.;
	byte gameState = 0;

	sf::View mainView;
	sf::View guiView;
	sf::SoundBuffer flightBuffer;
	sf::Sound flightSound;
	sf::Clock fuelClock;

	void doCollision(Player*);
};