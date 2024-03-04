#include "Character.h"


/// <summary>
/// Create new character who will have sprite from spritesheet.
/// </summary>
/// <param name="spriteSheet"></param>
Character::Character(sf::Texture* spriteSheet) : Entity()
{
	this->spriteSheet = spriteSheet;
	this->sprite.setTexture(*this->spriteSheet);
}


/// <summary>
/// Move the character by velocity and redraw them to the screen.
/// </summary>
/// <param name="window"></param>
void Character::update(sf::RenderWindow& window)
{
	// update the character's position using it's velocity
	setPos(getPos() + this->velocity);

	sprite.setPosition(getTranslate2() ? translateTo2d2(getPos()) : translateTo2d(getPos()));
	window.draw(sprite);
}


/// <summary>
/// Get the currently fired bullets from this character.
/// </summary>
/// <returns>Vector of bullets</returns>
std::vector<sf::Sprite>& Character::getBullets()
{
	return bullets;
}


/// <summary>
/// Get the height sector that character is in.
/// </summary>
/// <param name="planeSizeIndex"></param>
void Character::getSizeIndex(unsigned int& planeSizeIndex)
{
	const int y = (int)getPos().y - ((float)yMax);
	const int qSize = (yMin - yMax) / 4;
	planeSizeIndex = 3;

	for (int i = 0; i < 4; i++)
		if (y < qSize * i && y < qSize * i + 1)
			planeSizeIndex = 3 - i;
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
/// Get positions for all of this characters bullets.
/// </summary>
/// <returns>Vector of Vectors of 3 floats</returns>
std::vector<sf::Vector3f>& Character::getBulletPosition()
{
	return bulletsPos;
}


/// <summary>
/// Get rid of bullet by index.
/// </summary>
/// <param name="bullet"></param>
void Character::killBullet(int bullet)
{
	bullets.erase(bullets.begin() + bullet);
	bulletsPos.erase(bulletsPos.begin() + bullet);
}


/// <summary>
/// Set position for the character itself.
/// </summary>
/// <param name="pos"></param>
void Character::setPos(sf::Vector3f pos)
{
	if (pos.y >= yMax)
		pos.y = yMax - 1;
	else if (pos.y <= yMin)
		pos.y = yMin + 1;
	Entity::setPos(pos);
}