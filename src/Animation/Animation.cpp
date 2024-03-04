#include "Animation.h"


/// <summary>
/// Initalize individual animation frames.
/// </summary>
Animation::Animation()
{
	// Setup death explosion frames
	frames[0] = sf::IntRect(184, 144, 31, 31);
	frames[1] = sf::IntRect(224, 155, 31, 20);

	// Alt death explosion frames
	frames[7] = sf::IntRect(7, 147, 31, 27);
	frames[6] = sf::IntRect(49, 147, 30, 28);
	frames[5] = sf::IntRect(81, 147, 22, 27);
	frames[4] = sf::IntRect(109, 147, 20, 28);
	frames[3] = sf::IntRect(137, 147, 20, 31);
	frames[2] = sf::IntRect(158, 147, 17, 27);
}


Animation::~Animation()
{
}


/// <summary>
/// Run the given animation on sprite.
/// </summary>
/// <param name="sprite"></param>
/// <param name="anim"></param>
void Animation::run(sf::Sprite& sprite, Anim anim)
{
	timer.restart();

	switch (anim)
	{
	case CHARACTER_DEATH:
		// will delete itself.
		new std::thread(&Animation::fCHARACTER_DEATH, this, std::ref(sprite));
		break;
	case ALT_DEATH:
		new std::thread(&Animation::fALT_DEATH, this, std::ref(sprite));
		break;
	}
}


/// <summary>
/// Death animation definition.
/// </summary>
/// <param name="sprite"></param>
void Animation::fCHARACTER_DEATH(sf::Sprite& sprite)
{
	state = 2;

	// TODO: Not the real character death
	while (timer.getElapsedTime().asSeconds() < 2)
	{
		int current = static_cast<int>(timer.getElapsedTime().asSeconds() * 2) % 2;
		sprite.setTextureRect(frames[current]);
	}

	state = 1;
	//sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}


/// <summary>
/// Alternate death animation definition
/// </summary>
/// <param name="sprite"></param>
void Animation::fALT_DEATH(sf::Sprite& sprite)
{
	state = 2;

	while (timer.getElapsedTime().asMilliseconds() < 500)
	{
		int current = (static_cast<int>((int)(timer.getElapsedTime().asMilliseconds() * 12)/1000) % 6) + 2;
		sprite.setTextureRect(frames[current]);
	}

	state = 1;
}


/// <summary>
/// Checks if current animation has finished running.
/// </summary>
/// <returns>A boolean</returns>
int Animation::getState()
{
	return state;
}