#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <vector>


/// <summary>
/// Class to handle (almost) all in-game animations.
/// </summary>
class Animation
{
public:
	enum Anim
	{
		CHARACTER_DEATH,
		ALT_DEATH,
		LAUNCH,
		BULLET_DEATH,
		WALLBULLET_DEATH,
		ROCKET_FLICKER,
		RESET
	};

	Animation();
	~Animation();

	// size index as an extra var for some animations
	void run(sf::Sprite*, Anim, unsigned int sizeIndex = 0);
	int getState() const;

private:
	void fCHARACTER_DEATH(sf::Sprite* sprite);
	void fALT_DEATH(sf::Sprite* sprite);
	void fLAUNCH(sf::Sprite* sprite);
	void fBULLET_DEATH(sf::Sprite* sprite);
	void fWALLBULLET_DEATH(sf::Sprite* sprite);
	void fROCKET_FLICKER(sf::Sprite* sprite);

	std::vector<std::thread*> threads;
	sf::Clock timer;
	sf::IntRect frames[26], revert;
	int state = 0;
	unsigned int spriteSizeIndex = 0;
	bool kill = false;
};
