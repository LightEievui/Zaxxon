#include "CharacterBullet.h"


/// <summary>
/// Create a bullet based on if player or enemy shot it. Players use 3f &
/// enemies use 2f, enemies run on 2f space while players are on 3f space
/// and it's not possible to convert from 2f to 3f,
/// so enemies have to pass their 2f position to the bullets. Just make sure
/// to pass the 2f if an enemy and the 3f will be ignored.
/// </summary>
/// <param name="spritesheet"></param>
/// <param name="spawnPos">Ignored if spawnPos2f is defined.</param>
/// <param name="sizeIndex">Size of bullet.</param>
/// <param name="type"></param>
/// <param name="spawnPos2f">Optional, pass through when enemy.</param>
CharacterBullet::CharacterBullet(sf::Texture* spritesheet,
                                 sf::Vector3f spawnPos,
                                 unsigned int sizeIndex, BulletType type,
                                 sf::Vector2f spawnPos2f
)
{
	sprite->setTexture(*spritesheet);
	sprite->setTextureRect(sf::IntRect(8 + 16 * sizeIndex, 47, 16, 8));
	sprite->setOrigin(0, 8);

	if (type == Player)
	{
		sprite->setColor(sf::Color(0, 222, 0));
		setPos(sf::Vector3f(spawnPos.x - 21.f, spawnPos.y + .02f * sizeIndex,
		                    spawnPos.z - 15.f));
		sprite->setPosition(translateTo2d(getPos()));
	}
	else if (type == Enemy)
	{
		sprite->setColor(sf::Color(222, 0, 0));
		sprite->setPosition(
			spawnPos2f + sf::Vector2f(-3.f * static_cast<int>(3U - sizeIndex),
			                          20));
	}
	this->sizeIndex = sizeIndex;
	this->type = type;
}


/// <summary>
/// Get public size index of this character bullet
/// </summary>
/// <returns></returns>
unsigned int CharacterBullet::getSizeIndex()
{
	return sizeIndex;
}


/// <summary>
/// Default kill, run the kill method with enemyDeath as parameter
/// </summary>
void CharacterBullet::kill()
{
	kill(EnemyDeath);
}


/// <summary>
/// The bullet hit something, run code to kill bullet based on what it hit.
/// </summary>
/// <param name="deathType"></param>
void CharacterBullet::kill(BulletDeathType deathType)
{
	sprite->setColor(sf::Color(255, 255, 255));
	//death conditions and animations based on what is dying
	switch (deathType)
	{
	case WallDeath:
		if (animations.getState() < 1)
			animations.run(sprite, Animation::Anim::WALLBULLET_DEATH,
			               sizeIndex);
		break;
	case EnemyDeath:
		// adjust position to align better
		if (this->type == Player)
			setPos(getPos() + sf::Vector3f(0, 0, -16));
		if (animations.getState() < 2)
			animations.run(sprite, Animation::Anim::BULLET_DEATH, sizeIndex);
	}
}


/// <summary>
/// Run the logic for this bullet and then draw it to screen
/// </summary>
/// <param name="window"></param>
void CharacterBullet::update(sf::RenderWindow& window, float gameSpeed)
{
	if (type == Player)
	{
		if (animations.getState() == 0)
		{
			translate(-6 * gameSpeed);
		}
	}
	//doesn't use translate because it uses 2d movement
	else if (type == Enemy)
		sprite->move(translateTo2d(sf::Vector3f(0, 0, 3 * gameSpeed)));

	if (animations.getState() == 1)
		alive = false;

	window.draw(*sprite);
}
