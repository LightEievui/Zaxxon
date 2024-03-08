#include "../Character.h"
#define PI 3.1415
#pragma once


class Enemy : public Character
{
public:
	Enemy(sf::Texture*, unsigned int, int);
	void update(sf::RenderWindow&);
	void kill() override;
	bool getTranslate2() { return true; };
	// Static so can be used from any context.
	// Will spawn a wave of enemies relative to the player.
	static void spawnWave(std::vector<Enemy*>&, sf::Texture*, int, unsigned int);
private:
	void runAI();
	// angle -> x/z coords
	sf::Vector2f angleTranslate(sf::Vector2f angle);

	sf::IntRect textures[2][4];
	sf::Clock alive;
	unsigned int id;
};