#pragma once
#include <SFML/Graphics.hpp>
#include "Util/Util.h"
#include "Entity/Entity.h"


/// <summary>
/// Class for other walls (blue lightning).
/// </summary>
class ZapWall : public Entity
{
public:
	ZapWall(sf::Texture*, sf::Vector3f);
	~ZapWall();

	void update(sf::RenderWindow&) override;

	void setStartPos(sf::Vector3f pos);
	bool isOnScreen();
	sf::Vector3f getStartPosition();

private:
	bool onScreen = false;
	sf::Vector3f startPosition;
	int count = 0;
	int offScreenCount = 0;
	bool drawn = false;
};
