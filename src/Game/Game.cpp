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
	sf::RenderWindow window(sf::VideoMode(500, 500), "Zaxxon");
    //Set frame rate limit to smooth out
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        background.drawBackground(window);
        window.display();
    }
}
