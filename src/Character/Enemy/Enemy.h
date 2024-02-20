#include "../Character.h"
#pragma once

class Enemy : public Character
{
public:
	Enemy(sf::Texture* texture, unsigned int id);
	void update(sf::RenderWindow& window);
	void kill() override;
	bool getTranslate2() { return true; };
private:
	void runAI();
	sf::IntRect textures[2][4];
	sf::Clock alive;
	unsigned int id;
};

