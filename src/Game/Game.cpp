#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Character/Player/Player.h"
#include "Character/Enemy/Enemy.h"
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
    window.setSize(sf::Vector2u((unsigned int) (224.f * scale), (unsigned int) (256.f * scale)));
    mainView.reset(sf::FloatRect(0, 0, 224, 224));
    mainView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 224./256.));
    window.setView(mainView);

    guiView.reset(sf::FloatRect(0, 0, 224, 256));
    guiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    sf::Texture spriteSheet;
    spriteSheet.loadFromFile("./res/fixed_spritesheet.png");

    obstacles.push_back(new Obstacle);
    obstacles.at(0)->create(sf::Vector3f(-120, 135.6, -100), &spriteSheet, 400, 1);

    GUI gui(&spriteSheet);

    background.create("BackgroundFull.png", sf::Vector2f(0, 224));
    Player *player = new Player(&spriteSheet);
    Enemy *enemy = new Enemy(&spriteSheet); // temp

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
            mainView.move(sf::Vector2f(.8f * gameSpeed, -.4f * gameSpeed));
        /*else
        {
            //reset whenever boss is defeated
            //mainView.reset(sf::FloatRect(0, 0, 224, 224));
        }*/

        doCollision(player);

        window.clear();

        background.drawBackground(window);
        obstacles.at(0)->update(window);
        enemy->update(window);

        window.setView(guiView);
         // TODO: update inSpace on whether background is space or not.
        player->update(window, true);
        gui.render(window, player->getPos().y, score);

        window.setView(mainView);

        window.display();
    }

    delete player;
    delete enemy; // temp
    for (unsigned int i = 0; i < obstacles.size(); i++)
        delete obstacles.at(i);
}


void Game::doCollision(Player* player)
{
    
    std::vector<sf::Vector3f> bulletPos;
    sf::Vector3f difference;

    sf::Vector3f planePos;
    planePos = player->getPos();
    
    for (int i = 0; i < obstacles.size(); i++)
    {
        if (obstacles.at(i)->isPresent())
        {
            bulletPos = (obstacles.at(i)->getBulletLocations());

            for (unsigned int bullets = 0; bullets < bulletPos.size(); bullets++)
            {
                difference = sf::Vector3f(abs(bulletPos.at(bullets).x - planePos.x),
                    abs(bulletPos.at(bullets).y - planePos.y),
                    abs(bulletPos.at(bullets).z - planePos.z));

                

                if (difference.x < 40 && difference.y < 20 && difference.z < 20)
                {
                    player->kill();
                    std::cout << "Hit";
                    obstacles.at(i)->bulletKill(bullets);
                }
            }
        }
    }
}