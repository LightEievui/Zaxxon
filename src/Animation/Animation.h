#pragma once
#include <SFML/Graphics.hpp>


class Animation
{
public:
	const enum Anim {
		CHARACTER_DEATH
	};

	Animation();
	~Animation();

	void run(sf::Sprite, Anim);

private:
	sf::Clock timer;
	sf::IntRect frames[2];
};

