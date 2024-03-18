#include "../Character.h"
#define PI 3.1415
#pragma once


/// <summary>
/// Enemy in the game. These use a recolored player bullet.
/// </summary>
class Enemy : public Character
{
public:
	Enemy(sf::Texture*, unsigned int, int);
	void update(sf::RenderWindow&, float gameSpeed);
	void kill() override;
	bool getTranslate2() { return true; };
	unsigned int getSizeIndex();
	// Static so can be used from any context.
	// Will spawn a wave of enemies relative to the player.
	static void spawnWave(std::vector<Enemy*>&, sf::Texture*, int, unsigned int);

private:
	// returns the velocity it has moved already
	sf::Vector2f runAI();
	// angle -> x/z coords
	sf::Vector2f angleTranslate(float angle, float scale);

	sf::IntRect textures[2][4];
	sf::Clock alive;
	unsigned int sizeIndex = 0;
	unsigned int id;
};