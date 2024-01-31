#include <SFML/Graphics.hpp>
#include "Game.h"

const double scale = 3;


Game::Game()
{
    
}


Game::~Game()
{

}


void Game::run()
{
    //added to constructor so that it is not created every frame
    sf::RenderWindow window(sf::VideoMode(256 * scale, 224 * scale), "Zaxxon");
    //Set frame rate limit to smooth out
    window.setFramerateLimit(60);

    background.create("Image.png", scale, sf::Vector2f(0, window.getSize().y), sf::Vector2f(-.8 * scale * gameSpeed, .4 * scale * gameSpeed));

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

        //resets the background when image is off of screen.
        if (background.backgroundFinished())
        {
            background.reset();
            gameSpeed += .2;
        }
    }
}
