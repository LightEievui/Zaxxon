#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Entity/Character/Player/Player.h"
#include "Entity/Character/Enemy/Enemy.h"
#include "GUI/GUI.h"


const float scale = 2;


Game::Game()
{
    
}


Game::~Game()
{
    
    const int obstaclesSize = obstacles.size();
    for (unsigned int i = 0; i < obstaclesSize; i++)
        delete obstacles[i];
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

    generateObstacles(&spriteSheet);

    //Testing for gavin
    obstacles.push_back(new Obstacle(sf::Vector3f(-120, 135.6, -700), &spriteSheet, 10, 1));
    //obstacles.at(0)->create(sf::Vector3f(-120, 135.6, -700), &spriteSheet, 10, 1);

    GUI gui(&spriteSheet);

    background.create("BackgroundFull.png", sf::Vector2f(0, 224));
    Player *player = new Player(&spriteSheet);
    std::vector<Enemy*> enemies;

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
        for (int i = 0; i < obstacles.size(); i++)
        {
            obstacles.at(i)->update(window);
        }

        for (Enemy* enemy : enemies)
            enemy->update(window);
        // TODO: update inSpace on whether background is space or not.
        player->update(window, true);
        window.setView(guiView);
        gui.render(window, player->getPos().y, score);
        window.setView(mainView);
        

        window.display();
    }

    delete player;
    const int enemiesSize = enemies.size();
    for (int i = 0; i < enemiesSize; i++)
        delete enemies[i];
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
            /*Player Bullets Hitting Obstacles
            difference = sf::Vector3f(abs(obstacles.at(i)->getPosition().x - planePos.x),
                abs(obstacles.at(i)->getPosition().y - planePos.y),
                abs(obstacles.at(i)->getPosition().z - planePos.z));

            if (difference.x < 40 && difference.y < 20 && difference.z < 20)
            {
                obstacles.at(i)->kill();
            }
            */
            //Bullets
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


void Game::generateObstacles(sf::Texture* spriteSheet)
{
    /*Shooting Obstacles 
    KEY
    0 = Grey Turrets
    1 = Green Turrets
    2 = Shooting Up Bullets
    */
    obstacles.push_back(new Obstacle(sf::Vector3f(-150, 135.6, -470), spriteSheet, 1, 1));
    
    //Testing
    obstacles.push_back(new Obstacle(sf::Vector3f(-100, 135.6, -700), spriteSheet, 1, 0));
    

    /*
    Stationary Obstacles
    KEY
    1 = gas can
    2 = satellite
    */
    obstacles.push_back(new Obstacle(sf::Vector3f(-157, 135.6, -425), spriteSheet, 2));
    obstacles.push_back(new Obstacle(sf::Vector3f(-83, 135.6, -625), spriteSheet, 1));
    obstacles.push_back(new Obstacle(sf::Vector3f(-30, 135.6, -630), spriteSheet, 1));
    obstacles.push_back(new Obstacle(sf::Vector3f(-150, 135.6, -745), spriteSheet, 1));
    obstacles.push_back(new Obstacle(sf::Vector3f(-150, 135.6, -995), spriteSheet, 1));
    obstacles.push_back(new Obstacle(sf::Vector3f(-30, 135.6, -990), spriteSheet, 1));
    obstacles.push_back(new Obstacle(sf::Vector3f(-65, 135.6, -1115), spriteSheet, 1));
    obstacles.push_back(new Obstacle(sf::Vector3f(-30, 135.6, -1290), spriteSheet, 1));
}