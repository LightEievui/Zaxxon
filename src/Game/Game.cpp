#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Character/Player/Player.h"
#include "GUI/GUI.h"


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

    Obstacle temp, newTemp;

    temp.createObstacle(sf::Vector3f(-100, 0, -500), "ZaxxonFull.png", true, 0);
    obstacles.push_back(temp);
    GUI gui(&spriteSheet);

    background.create("BackgroundFull.png", sf::Vector2f(0, 224));
    Player player(&spriteSheet);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        view.move(sf::Vector2f(.8, -.4));

        window.clear();
        background.drawBackground(window);
        obstacles.at(0).drawObstacle(window);
        player.update(window);
        gui.render(window, player.getPos().y);
        window.setView(view);
        window.display();

    }
}
