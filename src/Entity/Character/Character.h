#pragma once
#include <SFML/Graphics.hpp>
#include "Util/Util.h"
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
	const int yMax = 140;
	const int yMin = 69;
	const int xMin = -150;
	const int xMax = 0;

	/* this computes the sizeindex (the quadrant the plane is in)
	using the currentY and the min / max,
	assumed there is 4 for everything inheriting character. 
	*/
	void getSizeIndex(unsigned int& planeSizeIndex);
	sf::Vector3f getVelocity();

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

