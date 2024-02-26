#include "Animation.h"


/// <summary>
/// Initalize individual animation frames.
/// </summary>
Animation::Animation()
{
	// Setup death explosion frames
	frames[0] = sf::IntRect(184, 144, 31, 31);
	frames[1] = sf::IntRect(224, 155, 31, 20);
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
	}
}


/// <summary>
/// Death animation definition.
/// </summary>
/// <param name="sprite"></param>
void Animation::fCHARACTER_DEATH(sf::Sprite& sprite)
{
	// TODO: Not the real character death
	while (timer.getElapsedTime().asSeconds() < 2)
	{
		int current = static_cast<int>(timer.getElapsedTime().asSeconds() * 2) % 2;
		sprite.setTextureRect(frames[current]);
	}

	complete = true;
	//sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}


/// <summary>
/// Checks if current animation has finished running.
/// </summary>
/// <returns>A boolean</returns>
bool Animation::getComplete()
{
	return complete;
}