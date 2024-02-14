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
    obstacles.at(0)->create(sf::Vector3f(-200, 0, -700), "ZaxxonFull.png", true, 0);

    GUI gui(&spriteSheet);

    background.create("BackgroundFull.png", sf::Vector2f(0, 224));
    Player *player = new Player(&spriteSheet);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // TODO: REMOVE THIS
            if (event.type == sf::Event::MouseButtonPressed)
                score += 100;
        }

        if (background.backgroundFinished(mainView) == false)
        {
            mainView.move(sf::Vector2f(.8 * gameSpeed, -.4 * gameSpeed));
        }
        /*else
        {
            //reset whenever boss is defeated
            //mainView.reset(sf::FloatRect(0, 0, 224, 224));
        }*/

        doCollision();

        window.clear();

        background.drawBackground(window);
        obstacles.at(0)->update(window);

        window.setView(guiView);
        player->update(window, true); // TODO: update inSpace on whether background is space or not.
        gui.render(window, player->getPos().y, score);
        window.setView(mainView);

        window.display();
    }

    for (int i = 0; i < obstacles.size(); i++)
    {
        delete obstacles.at(i);
    }
}


void Game::doCollision()
{
    std::vector<sf::Vector3f> bulletPos;

    for (int i = 0; i < obstacles.size(); i++)
    {
        if (obstacles.at(i)->isPresent())
        {
            //bulletPos = obstacles.at(i)->getBulletLocations();

            for (int bullets = 0; bullets < bulletPos.size(); bullets++)
            {
                // TODO: Something here?
            }
        }
    }
}
