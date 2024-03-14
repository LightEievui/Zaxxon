#include "CharacterBullet.h"


CharacterBullet::CharacterBullet(sf::Texture* spritesheet, sf::Vector3f spawnPos, unsigned int sizeIndex, BulletType type)
{
	sprite.setTexture(*spritesheet);
	sprite.setTextureRect(sf::IntRect(8 + 16 * sizeIndex, 47, 16, 8));
	sprite.setOrigin(0, 8);

	if(type == Player)
		setPos(sf::Vector3f(spawnPos.x - 21, spawnPos.y + .02 * sizeIndex, spawnPos.z - 15));
	//else if (type == Enemy) ...
	sprite.setPosition(translateTo2d(getPos()));
	this->sizeIndex = sizeIndex;
	this->type = type;
}

void CharacterBullet::update(sf::RenderWindow& window)
{
	if (type == Player)
		setPos(getPos() + sf::Vector3f(0, 0, -6));

	sprite.setPosition(translateTo2d(getPos()));
	window.draw(sprite);
}
