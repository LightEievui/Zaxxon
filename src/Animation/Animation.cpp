#include "Animation.h"


Animation::Animation()
{
	// Setup death explosion frames
	frames[0] = sf::IntRect(184, 144, 31, 31);
	frames[1] = sf::IntRect(224, 155, 31, 20);
}


Animation::~Animation()
{
}


void Animation::run(sf::Sprite& sprite, Anim anim)
{
	timer.restart();

	switch (anim)
	{
	case CHARACTER_DEATH:
		// will delete itself.
		new std::thread(&Animation::fCHARACTER_DEATH, this, std::ref(sprite));
		break;
	}
}


// TODO: Not the real character death
void Animation::fCHARACTER_DEATH(sf::Sprite& sprite)
{
	while (timer.getElapsedTime().asSeconds() < 2)
	{
		int current = static_cast<int>(timer.getElapsedTime().asSeconds() * 2) % 2;
		sprite.setTextureRect(frames[current]);
	}

	sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}
