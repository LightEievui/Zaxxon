#include "Entity.h"


/// <summary>
/// Setup Entity class variables
/// </summary>
Entity::Entity()
{
	this->sprite = new sf::Sprite();
	this->spriteSheet = nullptr;

	debugFont.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	debugText.setFont(debugFont);
	debugText.setScale(sf::Vector2f(0.2f, 0.2f));
}


/// <summary>
/// Clean up memory related to Entity class
/// </summary>
Entity::~Entity()
{
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
