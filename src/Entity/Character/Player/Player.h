#pragma once
#include <SFML/Audio.hpp>
#include "../Character.h"
#include "Entity/AbstractBullet/CharacterBullet/CharacterBullet.h"


class Player : public Character
{
public:
	Player(sf::Texture*, unsigned int);
	void update(sf::RenderWindow&, int);
	void kill() override;
	void resetPos(int zOffset = 0);
	void bulletKill(int);
	bool isAlive();

private:
	const int BULLET_COOLDOWN = 200;
	bool alive = true;

	sf::Clock bulletCD;
	sf::IntRect playerTextures[3][4];
	sf::Sprite shadow;
	sf::SoundBuffer bulletBuffer;
	sf::Sound bulletSound;
};