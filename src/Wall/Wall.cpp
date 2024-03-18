#include "Wall.h"


Wall::Wall(sf::Texture* tex, sf::Vector3f pos, int size, std::vector<int> walls)
{
    spriteSheet = tex;

    /*
    Closed wall setTextureRect(sf::IntRect(240, 64, 34, 32));
    Open Wall setTextureRect(sf::IntRect(278, 65, 34, 32));
    KEY
    1 = closed wall piece
    2 = open wall piece
    */

    for (int i = 0; i < size; i++) 
    {
        sprites.push_back(sf::Sprite());
        sprites.at(i).setTexture(*spriteSheet);
        sprites.at(i).setPosition(translateTo2d(sf::Vector3f(pos.x - 38 * i, pos.y, pos.z)));
        wallPositions.push_back(sf::Vector3f(pos.x - 38 * i, pos.y, pos.z));

        if (walls.at(i) == 0)
            sprites.at(i).setTextureRect(sf::IntRect(240, 64, 33, 31));
        else
            sprites.at(i).setTextureRect(sf::IntRect(279, 64, 33, 31));
    }
}


Wall::~Wall()
{

}


void Wall::drawWalls(sf::RenderWindow& window)
{
    if (!getWindowViewRect(window).intersects(sprites.at(sprites.size() - 1).getGlobalBounds()))
    {
        onScreen = false;
        return;
    }

    onScreen = true;

    for (unsigned int i = 0; i < sprites.size(); i++)
        window.draw(sprites.at(i));

}


void Wall::setPosition(sf::Vector3f pos, int piece)
{
    sprites.at(piece).setPosition(translateTo2d(pos));
    wallPositions.at(piece) = pos;
}


void Wall::setTexture(int piece, int tex)
{
    //Closed Piece
    if (tex == 0)
        sprites.at(piece).setTextureRect(sf::IntRect(240, 64, 34, 32));

    //Closed Piece
    if (tex == 1)
        sprites.at(piece).setTextureRect(sf::IntRect(278, 65, 34, 32));
}


bool Wall::checkOnScreen()
{
    return onScreen;
}


//Returns 3d Position of first section of wall
std::vector<sf::Vector3f> Wall::getWallPositions()
{
    return wallPositions;
}