#include <SFML/Graphics.hpp>
#include "Game.h"


Game::Game()
{
	
}


Game::~Game()
{

}


void Game::run()
{
	sf::RenderWindow window(sf::VideoMode(256, 224), "Zaxxon");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.display();
    }
}