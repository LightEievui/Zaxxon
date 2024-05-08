#pragma once
#include <SFML/Graphics.hpp>
#include "Util/Util.h"
#include "Animation/Animation.h"
#include "Entity/Entity.h"
#include "Entity/AbstractBullet/CharacterBullet/CharacterBullet.h"


/// <summary>
/// Abstract class to hold information that both player and enemy use.
/// </summary>
class Character : public Entity
{
public:
	Character(sf::Texture*);
	~Character() override;
	void update(sf::RenderWindow&, float) override;
	virtual void kill() = 0;

	std::vector<CharacterBullet*>& getBullets();
	unsigned int getSizeIndex() const;
	void killBullet(int);

	void setPos(sf::Vector3f);
protected:
	const int yMax = 140;
	const int yMin = 69;
	const int xMin = -160;
	const int xMax = 0;

	void _getSizeIndex(unsigned int&);
	sf::Vector3f getVelocity() const;

	void setVelocity(sf::Vector3f);
	void setBullet(sf::IntRect);

	void updateBullets(sf::RenderWindow& window, float);

	sf::Vector3f velocity;
	std::vector<CharacterBullet*> bullets;
	unsigned int sizeIndex;
private:
	sf::IntRect bulletTexture;
};
