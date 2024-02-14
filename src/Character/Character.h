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
	void update(sf::RenderWindow& window);
	virtual void kill() = 0;

	std::vector<sf::Sprite>& getBullets();
	virtual bool getTranslate2() { return false; };
protected:
	void setVelocity(sf::Vector3f vel);
	void setBullet(sf::IntRect bulletTextureRect);
	void setPos(sf::Vector3f pos);

	sf::Vector3f velocity;
	std::vector<sf::Sprite> bullets;
private:
	sf::IntRect bulletTexture;
	// SoundBuffer buffer;
	// Sound sound;
};

