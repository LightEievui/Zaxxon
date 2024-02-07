#pragma once
#include "../Character.h"


class Player : public Character
{
public:
	Player(sf::Texture* texture);
	void update(sf::RenderWindow& window);
	void kill();
	bool getMoveWithView();
private:
	sf::Sprite shadow;
};

