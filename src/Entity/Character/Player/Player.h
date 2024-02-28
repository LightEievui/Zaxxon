#pragma once
#include <SFML/Audio.hpp>
#include "../Character.h"


class Player : public Character
{
public:
	Player(sf::Texture* texture, unsigned int startPos);
	void update(sf::RenderWindow& window, bool inSpace);
	void kill() override;
	bool getTranslate2();
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