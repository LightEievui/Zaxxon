#include "Animation.h"
typedef void(Animation::* animation)(sf::Sprite*);

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

	// Wall bullet Death animation
	frames[10] = sf::IntRect(288, 146, 28, 30);
	frames[11] = sf::IntRect(384, 320, 27, 30);

	// Normal bullet death animations
	frames[12] = sf::IntRect(160, 161, 174-160, 175-161);
	frames[13] = sf::IntRect(136, 160, 151-136, 175-160);
	frames[14] = sf::IntRect(112, 158, 128-112, 175-158);
}


Animation::~Animation()
{
}


/// <summary>
/// Run the given animation on sprite. Note that you should always check if the sprite* is null before doing operations.
/// </summary>
/// <param name="sprite">Nullable</param>
/// <param name="anim"></param>
void Animation::run(sf::Sprite* sprite, Anim anim)
{
	timer.restart();

	// safety check
	if (sprite == nullptr)
		return;

	//get the original texture so that the RESET animation can do so.
	revert = sprite->getTextureRect();
	kill = false;

	animation animationPtr = nullptr;
	switch (anim)
	{
	case CHARACTER_DEATH:
		animationPtr = &Animation::fCHARACTER_DEATH;
		break;
	case ALT_DEATH:
		animationPtr = &Animation::fALT_DEATH;
		break;
	case BULLET_DEATH:
		animationPtr = &Animation::fBULLET_DEATH;
		break;
	case LAUNCH:
		animationPtr = &Animation::fLAUNCH;
		break;
	case RESET:
		kill = true;
		animationPtr = &Animation::fRESET;
		break;
	}

	// will delete itself.
	new std::thread(animationPtr, this, sprite);
}


/// <summary>
/// Death animation definition.
/// </summary>
/// <param name="sprite"></param>
void Animation::fCHARACTER_DEATH(sf::Sprite* sprite)
{
	state = 2;

	// TODO: Not the real character death
	while (timer.getElapsedTime().asSeconds() < 2 && sprite != nullptr)
	{
		int current = static_cast<int>(timer.getElapsedTime().asSeconds() * 2) % 2;
		sprite->setTextureRect(frames[current]);
	}

	kill = false;
	state = 1;
	//sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}


/// <summary>
/// Alternate death animation definition
/// </summary>
/// <param name="sprite"></param>
void Animation::fALT_DEATH(sf::Sprite* sprite)
{
	state = 2;

	while (timer.getElapsedTime().asMilliseconds() < 500 && sprite != nullptr)
	{
		int current = (static_cast<int>((int)(timer.getElapsedTime().asMilliseconds() * 12) / 1000) % 6) + 2;
		sprite->setTextureRect(frames[current]);
	}

	kill = false;
	state = 1;
}


/// <summary>
/// missle launch animation definition
/// </summary>
/// <param name="sprite"></param>
void Animation::fLAUNCH(sf::Sprite* sprite)
{
	state = 3;
	int current = 8;

	while (timer.getElapsedTime().asSeconds() < 2 && sprite != nullptr)
	{
		if (timer.getElapsedTime().asMilliseconds() > 500 && current == 8)
			current++;

		sprite->setTextureRect(frames[current]);
	}

	kill = false;
}


/// <summary>
/// Bullet on collision animation.
/// </summary>
/// <param name="sprite"></param>
void Animation::fBULLET_DEATH(sf::Sprite* sprite)
{
	int current = 10;
	state = 2;

	while (timer.getElapsedTime().asSeconds() < 2 && sprite != nullptr)
	{
		if (timer.getElapsedTime().asMilliseconds() > 500 && current == 8)
			current++;

		if (sprite == nullptr)
			return;
		sprite->setTextureRect(frames[current]);
	}

	state = 1;
}


void Animation::fWALLBULLET_DEATH(sf::Sprite* sprite)
{

}


/// <summary>
/// Resets the sprite too what it was before the last animation
/// </summary>
/// <param name="sprite"></param>
void Animation::fRESET(sf::Sprite* sprite)
{
	state = 0;
	sprite->setTextureRect(revert);
}


/// <summary>
/// Checks if current animation has finished running.
/// </summary>
/// <returns>A boolean</returns>
int Animation::getState()
{
	return state;
}