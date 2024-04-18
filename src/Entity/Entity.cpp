#include "Entity.h"


/// <summary>
/// Setup Entity class variables
/// </summary>
Entity::Entity()
{
	this->sprite = new sf::Sprite();
	this->spriteSheet = new sf::Texture();
}


/// <summary>
/// Clean up memory related to Entity class
/// </summary>
Entity::~Entity()
{
	if(sprite != nullptr)
		delete sprite;
}


/// <summary>
/// Run death animation code for the killed entity.
/// </summary>
void Entity::kill(Animation::Anim animation)
{
	animations.run(sprite, animation);
}


/// <summary>
/// Get the position of the related sprite for this entity
/// </summary>
/// <returns></returns>
sf::Vector2f Entity::getSpritePos()
{
	return sprite->getPosition();
}