#pragma once
#include "Animation/Animation.h"
#include <SFML/Graphics.hpp>


class Entity
{
public:
	Entity();
	~Entity();

	virtual void kill(Animation::Anim animation = Animation::CHARACTER_DEATH);
	virtual void update(sf::RenderWindow&) = 0;

	void setPos(sf::Vector3f pos) { position = pos; };
	sf::Vector3f getPos() { return position; };
	sf::FloatRect getBounds() { return sprite->getGlobalBounds(); };

protected:
	sf::Texture* spriteSheet;
	sf::Sprite* sprite;
	Animation animations;
	bool dead = false;

private:
	sf::Vector3f position;
};