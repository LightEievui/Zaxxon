#pragma once
#include <SFML/Graphics.hpp>
#include "Util/Util.h"
#include "Animation/Animation.h"
#include "Entity/Entity.h"


// Abstract
class Character : public Entity
{
public:
	Character(sf::Texture*);
	void update(sf::RenderWindow&);
	virtual void kill() = 0;
	
	std::vector<sf::Vector3f>& getBulletPosition();
	std::vector<sf::Sprite>& getBullets();
	unsigned int getSizeIndex();
	void killBullet(int);

	void setPos(sf::Vector3f);
protected:
	const int yMax = 140;
	const int yMin = 69;
	const int xMin = -160;
	const int xMax = 0;

	/* 
	This computes the sizeindex (the quadrant the plane is in)
	using the currentY and the min / max,
	assumed there is 4 for everything inheriting character. 
	*/
	void _getSizeIndex(unsigned int&);
	sf::Vector3f getVelocity();

	void setVelocity(sf::Vector3f);
	void setBullet(sf::IntRect);

	sf::Vector3f velocity;
	std::vector<sf::Sprite> bullets;
	std::vector<sf::Vector3f> bulletsPos;
	unsigned int sizeIndex;
private:
	sf::IntRect bulletTexture;
	// SoundBuffer buffer;
	// Sound sound;
};