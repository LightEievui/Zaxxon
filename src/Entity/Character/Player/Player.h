#pragma once
#include <SFML/Audio.hpp>
#include "Entity/Character/Character.h"
#include "Entity/AbstractBullet/CharacterBullet/CharacterBullet.h"


class Player : public Character
{
public:
	Player(sf::Texture*, unsigned int);
	void update(sf::RenderWindow&, int);
	void kill() override;
	void resetPos(int zOffset = 0);
	void drawHitmarker() { hitmarkerTimer.restart(); };

	bool isAlive();
private:
	const int BULLET_COOLDOWN = 200;
	bool alive = true;

	sf::Sprite hitmarker;
	sf::Clock bulletCD;
	sf::Clock hitmarkerTimer;
	sf::IntRect playerTextures[3][4];
	sf::Sprite shadow;
	sf::SoundBuffer bulletBuffer;
	sf::Sound bulletSound;
};