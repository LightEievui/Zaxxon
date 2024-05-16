#include "Wall.h"


/// <summary>
/// Create a section of walls, each one can be a closed or open wall piece
/// </summary>
/// <param name="tex"></param>
/// <param name="pos"></param>
/// <param name="size"></param>
/// <param name="walls"></param>
Wall::Wall(sf::Texture* tex, sf::Vector3f pos, int size, std::vector<int> walls)
{
	spriteSheet = tex;

	/*
	KEY for Vector
	0 = closed wall piece
	1 = open wall piece
	*/

	for (int i = 0; i < size; i++)
	{
		sprites.push_back(sf::Sprite());
		sprites.at(i).setTexture(*spriteSheet);
		sprites.at(i).setPosition(
			translateTo2d(sf::Vector3f(pos.x - 38 * i, pos.y, pos.z)));
		wallPositions.push_back(sf::Vector3f(pos.x - 38 * i, pos.y, pos.z));

		if (walls.at(i) == 0)
			sprites.at(i).setTextureRect(sf::IntRect(240, 64, 33, 31));
		else
			sprites.at(i).setTextureRect(sf::IntRect(279, 64, 33, 31));
	}
}


/// <summary>
/// Draw each section of the wall if it will be visible on the screen
/// </summary>
/// <param name="window"></param>
void Wall::drawWalls(sf::RenderWindow& window)
{
	if (!getWindowViewRect(window).intersects(
			sprites.at(sprites.size() - 1).getGlobalBounds()) &&
		!getWindowViewRect(window).intersects(sprites.at(0).getGlobalBounds()))
	{
		onScreen = false;
		return;
	}

	onScreen = true;

	for (unsigned int i = 0; i < sprites.size(); i++)
		window.draw(sprites.at(i));
}


/// <summary>
/// Set position of specificed wall piece
/// </summary>
/// <param name="pos"></param>
/// <param name="piece"></param>
void Wall::setPosition(sf::Vector3f pos, int piece)
{
	sprites.at(piece).setPosition(translateTo2d(pos));
	wallPositions.at(piece) = pos;
}


/// <summary>
/// Set texture for if this wall piece should be closed or open
/// </summary>
/// <param name="piece"></param>
/// <param name="tex"></param>
void Wall::setTexture(int piece, int tex)
{
	//Closed Piece
	if (tex == 0)
		sprites.at(piece).setTextureRect(sf::IntRect(240, 64, 34, 32));

	//Open Piece
	if (tex == 1)
		sprites.at(piece).setTextureRect(sf::IntRect(278, 65, 34, 32));
}


/// <summary>
/// Check if this wall will be on screen
/// </summary>
/// <returns>A boolean</returns>
bool Wall::checkOnScreen()
{
	return onScreen;
}


/// <summary>
/// Get 3d position of this wall
/// </summary>
/// <returns>Vector of 3 float vectors</returns>
std::vector<sf::Vector3f>& Wall::getWallPositions()
{
	return wallPositions;
}
