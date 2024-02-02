#pragma once
#include <SFML/Graphics.hpp>
#include "../Util/Util.h"
#include "../Animation.h"


// abstract
class Character
{
public:
	Character(sf::Texture* spriteSheet);
	virtual void update(sf::RenderWindow& window);
	virtual void kill() = 0;

	void setVelocity(sf::Vector3f vel);
	void setBullet(sf::IntRect bulletTextureRect);
	
	sf::Vector3f getPos();
	std::vector<sf::Sprite>& getBullets();
protected:
	sf::Sprite sprite;
	sf::Vector3f position;
	sf::Vector3f velocity;
	Animation animations;
private:
	sf::Texture* spriteSheet;
	sf::IntRect bulletTexture;
	std::vector<sf::Sprite> bullets;
	sf::IntRect move[4]; // TODO: Change.
	// SoundBuffer buffer;
	// Sound sound;
};

