#pragma once
#include <SFML/Audio.hpp>
#include "Entity/Character/Character.h"
#include "Entity/AbstractBullet/CharacterBullet/CharacterBullet.h"


/// <summary>
/// The player character, also contains plane controls.
/// </summary>
class Player : public Character
{
public:
	Player(sf::Texture*, unsigned int);
	void update(sf::RenderWindow&, int, float);
	void kill() override;
	void resetPos(int zOffset = 0);
	void drawHitmarker() { hitmarkerTimer.restart(); }

	bool isAlive();
	bool isMissileable(); // if the player should be shot by missile

	void restartMissileTimer() { missileTimer.restart(); };

private:
	const int BULLET_COOLDOWN = 200;
	bool alive = true;

	sf::Sprite hitmarker;
	sf::Clock bulletCD, hitmarkerTimer, missileTimer;
	sf::IntRect playerTextures[3][4];
	sf::Sprite shadow;
	sf::SoundBuffer bulletBuffer;
	sf::Sound bulletSound;
};
