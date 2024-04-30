#include "Character.h"


/// <summary>
/// Create new character who will have sprite from spritesheet.
/// </summary>
/// <param name="spriteSheet"></param>
Character::Character(sf::Texture* spriteSheet) : Entity()
{
	this->spriteSheet = spriteSheet;
	this->sprite->setTexture(*this->spriteSheet);
}


/// <summary>
/// Clean up memory related to the Character class
/// </summary>
Character::~Character()
{
	for (unsigned int i = 0; i < bullets.size(); i++)
		delete bullets[i];
}


/// <summary>
/// Move the character by velocity and redraw them to the screen.
/// </summary>
/// <param name="window"></param>
void Character::update(sf::RenderWindow& window)
{
	// update the character's position using it's velocity
	setPos(getPos() + this->velocity);

	sprite->setPosition(translateTo2d(getPos()));
	window.draw(*sprite);

	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		if (bullets.at(i)->isHit())
		{
			bullets.erase(bullets.begin() + i);
		}
	}
}


/// <summary>
/// Get the currently fired bullets from this character.
/// </summary>
/// <returns>Vector of bullets</returns>
std::vector<CharacterBullet*>& Character::getBullets()
{
	return bullets;
}


/// <summary>
/// Get public size index of this character
/// </summary>
/// <returns></returns>
unsigned int Character::getSizeIndex()
{
	return sizeIndex;
}


/// <summary>
/// Get the height sector that character is in.
/// Underscore infront indicates that it is not public.
/// 0-3 3 indicates smallest, 0 indicates biggest.
/// </summary>
/// <param name="planeSizeIndex"></param>
void Character::_getSizeIndex(unsigned int& planeSizeIndex)
{
	const float y = abs(getPos().y - ((float)yMax));
	const float qSize = abs((yMax - yMin) / 4.f);
	planeSizeIndex = 3; // smallest

	for (int i = 1; i < 4; i++)
		if (y > qSize * i && y < qSize * (i + 1))
			planeSizeIndex = 3 - i;

	if (getPos().y >= yMax)
		planeSizeIndex = 3;
	if (getPos().y <= yMin)
		planeSizeIndex = 0;
}


/// <summary>
/// Get the velocity of character.
/// </summary>
/// <returns>Vector of 3 floats</returns>
sf::Vector3f Character::getVelocity()
{
	return velocity;
}


/// <summary>
/// Change how the bullets from this character look.
/// </summary>
/// <param name="bulletTexture"></param>
void Character::setBullet(sf::IntRect bulletTexture)
{
	this->bulletTexture = bulletTexture;
}


/// <summary>
/// Change the velocity of this character.
/// </summary>
/// <param name="vel"></param>
void Character::setVelocity(sf::Vector3f vel)
{
	this->velocity = vel;
}

/// <summary>
/// Calls CharacterBullet::update() on each bullet & deletes bullets that are not in the window.
/// </summary>
/// <param name="window">Main render window</param>
void Character::updateBullets(sf::RenderWindow& window)
{
	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		CharacterBullet* bullet = bullets.at(i);
		bullet->update(window);

		if (!getWindowViewRect(window).intersects(bullet->getBounds()) || bullet->getAnimationState() == 1)
		{
			delete bullet;
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}
}

/// <summary>
/// Get rid of bullet by index.
/// </summary>
/// <param name="bullet"></param>
void Character::killBullet(int bullet)
{
	bullets.erase(bullets.begin() + bullet);
	//bulletsPos.erase(bulletsPos.begin() + bullet);
}


/// <summary>
/// Set position for the character itself.
/// </summary>
/// <param name="pos"></param>
void Character::setPos(sf::Vector3f pos)
{
	if (pos.y >= yMax)
		pos.y = yMax - 1.f;
	else if (pos.y <= yMin)
		pos.y = yMin + 1.f;

	Entity::setPos(pos);
	sprite->setPosition(translateTo2d(getPos()));
}