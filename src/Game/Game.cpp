#include "Game.h"

const float scale = 2;
const unsigned int startPos = 0;


Game::Game()
    : window(sf::VideoMode(224, 256), "Zaxxon"), gui(&spriteSheet)
{
    spriteSheet.loadFromFile("./res/fixed_spritesheet.png");
    //Set frame rate limit to smooth out
    window.setFramerateLimit(60);

    // Resize window to scale, resize everything else with it using view
    window.setSize(sf::Vector2u((unsigned int)(224.f * scale), (unsigned int)(256.f * scale)));
    mainView.reset(sf::FloatRect(0.f, 0.f, 224.f, 224.f));
    mainView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 224. / 256.));
    window.setView(mainView);

    guiView.reset(sf::FloatRect(0.f, 0.f, 224.f, 256.f));
    guiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    flightBuffer.loadFromFile("res/sfx/02.wav");
    flightSound.setBuffer(flightBuffer);
    flightSound.setLoop(true);
    flightSound.play();

    //Testing for gavin
    obstacles.push_back(new Obstacle(sf::Vector3f(-120.f, 135.6f, -3.f
    ), &spriteSheet, 10, 2));
    //obstacles.at(0)->create(sf::Vector3f(-120, 135.6, -700), &spriteSheet, 10, 1);

    player = new Player(&spriteSheet, startPos);
    mainView.move(sf::Vector2f(.8f * startPos, -.4f * startPos));

    // MAY want to consider changing these to be in Background constructor
    Background::generateObstacles(Background::INITIAL, obstacles, &spriteSheet);
    Background::generateWaves(Background::INITIAL, enemies, &spriteSheet, player->getPos().z);
}


Game::~Game()
{
    const int obstaclesSize = obstacles.size();
    for (int i = 0; i < obstaclesSize; i++)
        delete obstacles[i];
}


void Game::run() // if random erros later check that stack isnt full
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // TODO: REMOVE THIS
            if (event.type == sf::Event::MouseButtonPressed)
                score += 100, fuel -= 1;
        }

        /*else
        {
            //reset whenever boss is defeated
            //mainView.reset(sf::FloatRect(0, 0, 224, 224));
        }*/

        doCollision(player);

        window.clear();

        background.update(window, mainView, gameSpeed, &spriteSheet, obstacles, enemies, *player);
        for (unsigned int i = 0; i < obstacles.size(); i++)
            obstacles.at(i)->update(window);

        for (Enemy* enemy : enemies)
            enemy->update(window);
        // TODO: update inSpace on whether background is space or not.
        player->update(window, false);
        window.setView(guiView);
        gui.render(window, player->getPos().y, score, fuel);
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
    int size;

    //Plane Bullets
    std::vector<sf::Vector3f> planeBulletPos;

    sf::Vector3f planePos;
    planePos = sf::Vector3f(player->getPos().x - 20, player->getPos().y, player->getPos().z - 20);

    for (unsigned int i = 0; i < obstacles.size(); i++)
    {

        if (obstacles.at(i)->isPresent())
        {
            //Bullets
            bulletPos = (obstacles.at(i)->getBulletLocations());

            for (unsigned int bullets = 0; bullets < bulletPos.size(); bullets++)
            {
                difference = sf::Vector3f(abs(bulletPos.at(bullets).x - planePos.x),
                    abs(bulletPos.at(bullets).y - planePos.y),
                    abs(bulletPos.at(bullets).z - planePos.z));

                if (difference.x < 10 && difference.y < 10 && difference.z < 10)
                {
                    player->kill();

                    obstacles.at(i)->bulletKill(bullets);
                }
            }
            bulletPos = player->getBulletPosition();
            size = bulletPos.size();

            //Player Bullets Hitting Obstacles -- This only really works with translateTo2d 
            for (unsigned int pBullets = 0; pBullets < size; pBullets++)
            {
                difference = sf::Vector3f(abs(obstacles.at(i)->getPosition().x - bulletPos.at(pBullets).x),
                    abs(obstacles.at(i)->getPosition().y - bulletPos.at(pBullets).y),
                    abs(obstacles.at(i)->getPosition().z - bulletPos.at(pBullets).z));

                if (difference.x < 20 && difference.y < 20 && difference.z < 20)
                {
                    obstacles.at(i)->kill();
                    player->killBullet(pBullets);
                    bulletPos.erase(bulletPos.begin() + pBullets);
                    pBullets--;
                    size--;
                }
            }
        }
    }
}