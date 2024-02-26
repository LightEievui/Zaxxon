#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>


class Animation
{
public:
	const enum Anim {
		CHARACTER_DEATH
	};

	Animation();
	~Animation();

	void run(sf::Sprite&, Anim);
	void fCHARACTER_DEATH(sf::Sprite& sprite);
	bool getComplete();
private:
	sf::Clock timer;
	sf::IntRect frames[2];
	bool complete = false; //if you want to change this that is fine, i just need some way to get when its done.
};

