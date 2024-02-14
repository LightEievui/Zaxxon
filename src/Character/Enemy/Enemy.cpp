#include "Enemy.h"


Enemy::Enemy(sf::Texture* texture) : Character(texture)
{
	// 96 37 25 25
	for (unsigned int i = 0; i < 2; i++)
		for (unsigned int j = 0; j < 4; j++)
			textures[i][j] = sf::IntRect(96 + 25*(i+j), 37, 25, 25);
	this->sprite.setTextureRect(textures[0][0]);
	this->setPos(sf::Vector3f(-200, 0, -500));
}


void Enemy::update(sf::RenderWindow& window)
{
	Character::update(window);
}


void Enemy::kill()
{

}