#include "Animation.h"


/// <summary>
/// Initalize individual animation frames.
/// </summary>
Animation::Animation()
{
	// Setup death explosion frames
	frames[0] = sf::IntRect(184, 144, 31, 31);
	frames[1] = sf::IntRect(224, 144, 31, 31);

	// Alt death explosion frames
	frames[7] = sf::IntRect(7, 147, 31, 27);
	frames[6] = sf::IntRect(49, 147, 30, 28);
	frames[5] = sf::IntRect(81, 147, 22, 27);
	frames[4] = sf::IntRect(109, 147, 20, 28);
	frames[3] = sf::IntRect(137, 147, 20, 31);
	frames[2] = sf::IntRect(158, 147, 17, 27);

	// Launch animation
	frames[9] = sf::IntRect(119, 71, 38, 27);
	frames[8] = sf::IntRect(159, 72, 38, 27);

	// Bullet Death animation
	frames[10] = sf::IntRect(288, 146, 28, 30);
	frames[11] = sf::IntRect(384, 320, 27, 30);
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

	//get the original texture so that the RESET animation can do so.

	switch (anim)
	{
	case CHARACTER_DEATH:
		// will delete itself.
		revert = sprite.getTextureRect();

		new std::thread(&Animation::fCHARACTER_DEATH, this, std::ref(sprite));
		break;
	case ALT_DEATH:
		revert = sprite.getTextureRect();

		new std::thread(&Animation::fALT_DEATH, this, std::ref(sprite));
		break;
	case LAUNCH:
		revert = sprite.getTextureRect();

		new std::thread(&Animation::fLAUNCH, this, std::ref(sprite));
		break;
	case RESET:
		kill = true;
		new std::thread(&Animation::fRESET, this, std::ref(sprite));
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
	while (timer.getElapsedTime().asSeconds() < 2 && kill == false)
	{
		int current = static_cast<int>(timer.getElapsedTime().asSeconds() * 2) % 2;
		sprite.setTextureRect(frames[current]);
	}
	
	kill = false;
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

	while (timer.getElapsedTime().asMilliseconds() < 500 && kill == false)
	{
		int current = (static_cast<int>((int)(timer.getElapsedTime().asMilliseconds() * 12)/1000) % 6) + 2;
		sprite.setTextureRect(frames[current]);
	}

	kill = false;
	state = 1;
}


/// <summary>
/// missle launch animation definition
/// </summary>
/// <param name="sprite"></param>
void Animation::fLAUNCH(sf::Sprite& sprite)
{
	state = 3;
	int current = 8;

	while (timer.getElapsedTime().asSeconds() < 2 && kill == false)
	{
		if (timer.getElapsedTime().asMilliseconds() > 500 && current == 8)
			current++;

		sprite.setTextureRect(frames[current]);
	}

	kill = false;
}


/// <summary>
/// Bullet on collision animation.
/// </summary>
/// <param name="sprite"></param>
void Animation::fBULLET_DEATH(sf::Sprite& sprite)
{
	int current = 10;

	while (timer.getElapsedTime().asSeconds() < 2 && kill == false)
	{
		if (timer.getElapsedTime().asMilliseconds() > 500 && current == 8)
			current++;

		sprite.setTextureRect(frames[current]);
	}
}


/// <summary>
/// Resets the sprite too what it was before the last animation
/// </summary>
/// <param name="sprite"></param>
void Animation::fRESET(sf::Sprite& sprite)
{
	state = 0;
	sprite.setTextureRect(revert);
}


/// <summary>
/// Checks if current animation has finished running.
/// </summary>
/// <returns>A boolean</returns>
int Animation::getState()
{
	return state;
}