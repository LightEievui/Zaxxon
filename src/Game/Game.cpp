#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Character/Player/Player.h"


const float scale = 2.5;


Game::Game()
{
    
}


Game::~Game()
{

}


void Game::run()
{
    
    //added to constructor so that it is not created every frame
    sf::RenderWindow window(sf::VideoMode(256, 224), "Zaxxon");
    //Set frame rate limit to smooth out
    window.setFramerateLimit(60);

    // Resize window to scale, resize everything else with it using view
    window.setSize(sf::Vector2u(256 * scale, 224 * scale));
    view.reset(sf::FloatRect(0, 0, 256, 224));
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    window.setView(view);
    

    sf::Texture spriteSheet;
    spriteSheet.loadFromFile("./res/spritesheet.png");

    Obstacle temp;

    temp.createObstacle(sf::Vector3f(-300, -100, -700), "ZaxxonFull.png", sf::Vector2f(-.8 * gameSpeed, .4 * gameSpeed), true, 0);
    obstacles.push_back(temp);

    background.create("BackgroundFull.png", sf::Vector2f(0, 224), sf::Vector2f(-.8 * gameSpeed, .4 * gameSpeed));
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
        window.setView(view);
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
