#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>


class Animation
{
public:
	const enum Anim {
		CHARACTER_DEATH,
		ALT_DEATH,
		LAUNCH
	};

	Animation();
	~Animation();

	void run(sf::Sprite&, Anim);
	void fCHARACTER_DEATH(sf::Sprite& sprite);
	void fALT_DEATH(sf::Sprite& sprite);
	void fLAUNCH(sf::Sprite& sprite);
	int getState();
private:
	sf::Clock timer;
	sf::IntRect frames[8];
	int state = 0; 
};

