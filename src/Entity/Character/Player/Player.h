#pragma once
#include <SFML/Audio.hpp>
#include "../Character.h"


class Player : public Character
{
public:
	Player(sf::Texture*, unsigned int);
	void update(sf::RenderWindow&, bool);
	void kill() override;
	void resetPos(int zOffset = 0);
	void bulletKill(int);

private:
	const int BULLET_COOLDOWN = 200;
	sf::Clock bulletCD;
	std::vector<int> erase;
	sf::IntRect playerTextures[3][4];
	sf::Sprite shadow;
	sf::SoundBuffer bulletBuffer;
	sf::Sound bulletSound;
};