#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Character/Player/Player.h"

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
    sf::Texture spriteSheet;
    spriteSheet.loadFromFile("./res/spritesheet.png");

    Obstacle temp;

    temp.createObstacle(sf::Vector3f(-300, -100, -700), "ZaxxonFull.png", scale, sf::Vector2f(-.8 * scale * gameSpeed, .4 * scale * gameSpeed), true, 0);
    obstacles.push_back(temp);

    background.create("BackgroundFull.png", scale, sf::Vector2f(0, window.getSize().y), sf::Vector2f(-.8 * scale * gameSpeed, .4 * scale * gameSpeed));
    Player player(&spriteSheet);

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
        obstacles.at(0).drawObstacle(window);
        player.update(window);
        window.display();

        //resets the background when image is off of screen.
        if (background.backgroundFinished())
        {
            background.reset();
            gameSpeed += .2;
            background.setSpeed(gameSpeed);
        }
    }
}
