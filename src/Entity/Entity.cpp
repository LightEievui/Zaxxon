#include "Entity.h"


Entity::Entity()
{

}


Entity::~Entity()
{

}


/// <summary>
/// Run death animation code for the killed entity.
/// </summary>
void Entity::kill()
{
	animations.run(this->sprite, Animation::CHARACTER_DEATH);
}