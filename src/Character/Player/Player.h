#pragma once
#include "../Character.h"


class Player : protected Character
{
public:
	Player(sf::Texture* texture);
	void update(sf::RenderWindow& window);
	void kill();
private:

};

