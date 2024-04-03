#include "Entity.h"


Entity::Entity()
{
	this->sprite = new sf::Sprite();
}


Entity::~Entity()
{

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