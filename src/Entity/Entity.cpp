#include "Entity.h"


Entity::Entity()
{

}


Entity::~Entity()
{

}


void Entity::kill()
{
	animations.run(this->sprite, Animation::CHARACTER_DEATH);
}