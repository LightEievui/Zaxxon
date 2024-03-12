#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>
#include <vector>


class Animation
{
public:
	const enum Anim {
		CHARACTER_DEATH,
		ALT_DEATH,
		LAUNCH,
		RESET
	};

	Animation();
	~Animation();

	void run(sf::Sprite&, Anim);
	void fCHARACTER_DEATH(sf::Sprite& sprite);
	void fALT_DEATH(sf::Sprite& sprite);
	void fLAUNCH(sf::Sprite& sprite);
	void fRESET(sf::Sprite& sprite);
	int getState();
private:
	sf::Clock timer;
	sf::IntRect frames[10], revert;
	int state = 0; 
	bool kill = false;
};

