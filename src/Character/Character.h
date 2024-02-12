#pragma once
#include <SFML/Graphics.hpp>
#include "../Util/Util.h"
#include "Animation/Animation.h"
#include "Entity/Entity.h"


// abstract
class Character : public Entity
{
public:
	Character(sf::Texture* spriteSheet);
	virtual void update(sf::RenderWindow& window);
	virtual void kill() = 0;

	std::vector<sf::Sprite>& getBullets();
	virtual bool getMoveWithView();
protected:
	void setVelocity(sf::Vector3f vel);
	void setBullet(sf::IntRect bulletTextureRect);
	void setPos(sf::Vector3f pos);

	sf::Vector3f velocity;
	std::vector<sf::Sprite> bullets;
private:
	sf::IntRect bulletTexture;
	sf::IntRect move[4]; // TODO: Change.
	// SoundBuffer buffer;
	// Sound sound;
};

