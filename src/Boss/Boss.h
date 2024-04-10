#pragma once
#include "SFML/Graphics.hpp"
#include "Entity/Entity.h"
#include "Util/Util.h"


class Boss : Entity
{
public:
	Boss(sf::Vector3f, Entity*, sf::Texture*, sf::Texture*);
	~Boss();

	void update(sf::RenderWindow&);

private:

	Entity* target;
	sf::Clock movementInt;
	int stages = 0;

	sf::Texture spriteSheet;
};