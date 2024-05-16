#include "../Character.h"
#define PI 3.1415f
#pragma once


/// <summary>
/// Enemy in the game. These use a recolored player bullet.
/// </summary>
class Enemy : public Character
{
public:
	Enemy(sf::Texture* texture, unsigned int id, sf::Vector3f spawnZ,
	      unsigned int reset, int randOffset = 0);
	void update(sf::RenderWindow&, float gameSpeed) override;
	void kill() override;
	bool getTranslate2() { return true; };
	unsigned int getSizeIndex();
	// Static so can be used from any context.
	// Will spawn a wave of enemies relative to the player.
	static void spawnWave(std::vector<Enemy*>&, sf::Texture*, int,
	                      unsigned int, unsigned int reset);
	bool isDead() { return dead; };
private:
	// returns the velocity it has moved already
	sf::Vector2f runAI();
	// angle -> x/z coords
	sf::Vector2f angleTranslate(float angle, float scale);

	sf::IntRect textures[2][4];
	sf::Clock alive;
	bool dead = false;
	unsigned int sizeIndex = 0;
	unsigned int id;
	int randOffset = 0;

	// bullets
	bool ableToFire = true;
	sf::Clock bulletCD;
};