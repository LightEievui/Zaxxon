#include "../Character.h"
#pragma once

class Enemy : public Character
{
public:
	Enemy(sf::Texture* texture, unsigned int id);
	void update(sf::RenderWindow& window);
	void kill() override;
	bool getTranslate2() { return true; };
	// Static so can be used from any context.
	// Will spawn a wave of enemies relative to the player.
	static void spawnWave(std::vector<Enemy*>& enemies, sf::Texture* spritesheet,
		unsigned int playerZ, unsigned int wave
	);
private:
	void runAI();
	sf::IntRect textures[2][4];
	sf::Clock alive;
	unsigned int id;
};

