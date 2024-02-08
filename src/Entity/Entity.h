#pragma once
#include "Animation/Animation.h"
#include <SFML/Graphics.hpp>


class Entity
{
public:
	Entity();
	~Entity();

	virtual void kill();

	virtual void update(sf::RenderWindow&) = 0;
	virtual sf::Vector3f getPos() { return sf::Vector3f(0, 0, 0); }
protected:
	sf::Texture* spriteSheet;
	sf::Sprite sprite;
	sf::Vector3f position;
	Animation animations;

};

