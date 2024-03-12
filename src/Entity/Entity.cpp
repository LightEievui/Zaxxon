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
void Entity::kill(Animation::Anim animation)
{
	animations.run(this->sprite, animation);
}