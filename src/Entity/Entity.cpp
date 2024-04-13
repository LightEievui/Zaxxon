#include "Entity.h"


Entity::Entity()
{
	this->sprite = new sf::Sprite();
	this->spriteSheet = new sf::Texture();
}


Entity::~Entity()
{
	if(sprite != nullptr)
		delete sprite;

	if (sprite != nullptr)
		delete spriteSheet;
}


/// <summary>
/// Run death animation code for the killed entity.
/// </summary>
void Entity::kill(Animation::Anim animation)
{
	animations.run(sprite, animation);
}

sf::Vector2f Entity::getSpritePos()
{
	return sprite->getPosition();
}