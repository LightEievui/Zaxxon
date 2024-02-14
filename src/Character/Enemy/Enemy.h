#include "../Character.h"
#pragma once

class Enemy : public Character
{
public:
	Enemy(sf::Texture* texture);
	void update(sf::RenderWindow& window);
	void kill() override;
private:
	sf::IntRect textures[2][4];
};

