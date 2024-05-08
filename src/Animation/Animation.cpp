#include "Animation.h"
using animation = void(Animation::*)(sf::Sprite*);

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
	frames[8] = sf::IntRect(119, 71, 38, 27);
	frames[9] = sf::IntRect(159, 72, 38, 27);

	// Wall bullet Death animation
	frames[10] = sf::IntRect(264, 154, 279 - 264, 175 - 154);
	frames[11] = sf::IntRect(288, 146, 28, 30);

	// Normal bullet death animations
	frames[12] = sf::IntRect(160, 161, 174 - 160, 175 - 161);
	frames[13] = sf::IntRect(136, 160, 151 - 136, 175 - 160);
	frames[14] = sf::IntRect(112, 158, 128 - 112, 175 - 158);
	frames[15] = sf::IntRect(80, 156, 99 - 80, 175 - 156);
	frames[16] = sf::IntRect(48, 151, 70 - 48, 175 - 151);
	frames[17] = sf::IntRect(8, 150, 34 - 8, 176 - 150);

	// Numbers for obstacles after death
	frames[18] = sf::IntRect(184, 121, 211 - 184, 135 - 121); // 0
	frames[19] = sf::IntRect(224, 113, 255 - 224, 136 - 113); // 200
	frames[20] = sf::IntRect(264, 113, 295 - 264, 135 - 113); // 500
	frames[21] = sf::IntRect(305, 113, 334 - 305, 135 - 113); // 1000

	// rocket flicker
	frames[22] = sf::IntRect(80, 70, 10, 25);
	frames[23] = sf::IntRect(102, 70, 10, 25);

	// red rocket flicker
	frames[24] = sf::IntRect(302, 197, 10, 25);
	frames[25] = sf::IntRect(324, 197, 10, 25);

	kill = false;
}


/// <summary>
/// Clean up memory for animation class
/// </summary>
Animation::~Animation()
{
	for (std::thread* thread : threads)
	{
		thread->join();
		delete thread;
	}
}


/// <summary>
/// Run the given animation on sprite. Note that you should always check 
/// if the sprite* is null before doing operations.
/// </summary>
/// <param name="sprite">Nullable</param>
/// <param name="anim"></param>
void Animation::run(sf::Sprite* sprite, Anim anim, unsigned int sizeIndex)
{
	timer.restart();
	this->spriteSizeIndex = sizeIndex;

	// safety check
	if (sprite == nullptr)
		return;

	//get the original texture so that the RESET animation can do so.
	revert = sprite->getTextureRect();

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
	case WALLBULLET_DEATH:
		animationPtr = &Animation::fWALLBULLET_DEATH;
		break;
	case LAUNCH:
		animationPtr = &Animation::fLAUNCH;
		break;
	case ROCKET_FLICKER:
		animationPtr = &Animation::fROCKET_FLICKER;
		break;
	case RESET:
		kill = true;
		break;
	}

	if (animationPtr != nullptr)
		threads.push_back(new std::thread(animationPtr, this, sprite));
}


/// <summary>
/// Death animation definition.
/// </summary>
/// <param name="sprite"></param>
void Animation::fCHARACTER_DEATH(sf::Sprite* sprite)
{
	state = 2;

	while (timer.getElapsedTime().asSeconds() < 1 && sprite != nullptr)
	{
		int current =
			static_cast<int>(timer.getElapsedTime().asSeconds() * 4) % 2;
		sprite->setTextureRect(frames[current]);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	int current = 18;

	switch (spriteSizeIndex)
	{
	case 1: // 200
		current = 19;
		break;
	case 2: // 500
		current = 20;
		break;
	case 3: // 1000
		current = 21;
		break;
	}

	state = 4; // wont interact

	while (timer.getElapsedTime().asSeconds() < 2 && sprite != nullptr)
	{
		sprite->setTextureRect(frames[current]);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	sprite->setTextureRect(frames[18]);
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
		int current = (timer.getElapsedTime().asMilliseconds()
			* 12 / 1000 % 6) + 2;
		sprite->setTextureRect(frames[current]);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

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

	while (timer.getElapsedTime().asSeconds() < 1 && sprite != nullptr)
	{
		if (timer.getElapsedTime().asMilliseconds() > 500 && current == 8)
			current++;

		sprite->setTextureRect(frames[current]);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	state = 4;
}


/// <summary>
/// Bullet on collision animation.
/// </summary>
/// <param name="sprite"></param>
void Animation::fBULLET_DEATH(sf::Sprite* sprite)
{
	int current = 10;
	bool lastWentUp = false;
	state = 2;

	switch (spriteSizeIndex)
	{
	case 0:
		current = 16;
		break;
	case 1:
	case 2:
		current = 14;
		break;
	case 3:
	default:
		current = 12;
		break;
	}

	while (timer.getElapsedTime().asSeconds() < 2 && sprite != nullptr)
	{
		if (timer.getElapsedTime().asMilliseconds() > 500 && current == 8)
			current++;

		// change every 1/2 second
		if (timer.getElapsedTime().asMilliseconds() % 500 == 0)
		{
			if (lastWentUp)
				current--;
			else
				current++;

			lastWentUp = !lastWentUp;
		}

		if (sprite == nullptr)
			return;
		sprite->setTextureRect(frames[current]);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	state = 1;
}


/// <summary>
/// Animation for when a bullet hits a wall
/// </summary>
/// <param name="sprite"></param>
void Animation::fWALLBULLET_DEATH(sf::Sprite* sprite)
{
	int current = 10;
	state = 2;

	while (timer.getElapsedTime().asSeconds() < 0.7 && sprite != nullptr)
	{
		if (timer.getElapsedTime().asMilliseconds() > 350)
			current = 10;
		else
			current = 11;

		if (sprite == nullptr)
			return;
		sprite->setTextureRect(frames[current]);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	state = 1;
}

/// <summary>
/// Animation for a upward shooting rocket obstacle.
/// </summary>
/// <param name="sprite"></param>
void Animation::fROCKET_FLICKER(sf::Sprite* sprite)
{
	state = 6;

	if (!kill && sprite != nullptr)
	{
		if (spriteSizeIndex == 0) // green
			sprite->setTextureRect(frames[22]);
		else // red
			sprite->setTextureRect(frames[24]);
	}
	else
		return;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	if (!kill && sprite != nullptr)
	{
		if (spriteSizeIndex == 0) // green
			sprite->setTextureRect(frames[23]);
		else // red
			sprite->setTextureRect(frames[25]);
	}
	else
		return;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	if (!kill)
		state = 7;
}


/// <summary>
/// Checks if current animation has finished running.
/// </summary>
/// <returns>A boolean</returns>
int Animation::getState() const
{
	return state;
}
