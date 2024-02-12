#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Character/Player/Player.h"
#include "GUI/GUI.h"


const float scale = 2;


Game::Game()
{
    
}


Game::~Game()
{

}


void Game::run()
{
    
    //added to constructor so that it is not created every frame
    sf::RenderWindow window(sf::VideoMode(224, 256), "Zaxxon");
    //Set frame rate limit to smooth out
    window.setFramerateLimit(60);

    // Resize window to scale, resize everything else with it using view
    window.setSize(sf::Vector2u(224 * scale, 256 * scale));
    mainView.reset(sf::FloatRect(0, 0, 224, 224));
    mainView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 224./256.));
    window.setView(mainView);

    guiView.reset(sf::FloatRect(0, 0, 224, 256));
    guiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    sf::Texture spriteSheet;
    spriteSheet.loadFromFile("./res/spritesheet.png");

    obstacles.push_back(new Obstacle);

    GUI gui(&spriteSheet);

    background.create("BackgroundFull.png", sf::Vector2f(0, 224));
    Entity *player = new Player(&spriteSheet);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        mainView.move(sf::Vector2f(.8, -.4));

        window.clear();
        background.drawBackground(window);
        obstacles.at(0)->update(window);
        window.setView(guiView);
        player->update(window);
        gui.render(window, player->getPos().y);
        window.setView(mainView);
        window.display();

    }
}


void Game::doCollision()
{

}
