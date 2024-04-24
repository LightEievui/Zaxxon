#pragma once
#include "Animation/Animation.h"
#include <SFML/Graphics.hpp>
#include <Util/Util.h>


class Entity
{
public:
	Entity();
	~Entity();

	virtual void kill(Animation::Anim animation = Animation::CHARACTER_DEATH);
	virtual void update(sf::RenderWindow&) = 0;

	int getAnimationState() { return animations.getState(); };
	void setPos(sf::Vector3f pos) { position = pos; };
	sf::Vector3f getPos() { return position; };
	sf::FloatRect getBounds() { return sprite->getGlobalBounds(); };
	sf::Vector2f getSpritePos();

protected:
	sf::Texture* spriteSheet;
	sf::Sprite* sprite;
	Animation animations;
	bool dead = false;
	sf::Text debugText;
private:
	sf::Vector3f position;
};