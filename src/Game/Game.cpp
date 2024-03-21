#include "Game.h"

const float scale = 2;
const unsigned int startPos = 0;
Background::Stage startStage = Background::BOSS;


/// <summary>
/// Initialize everything needed for the main game functionality.
/// </summary>
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

    player = new Player(&spriteSheet, startPos);
    mainView.move(sf::Vector2f(.8f * startPos, -.4f * startPos));

    // Ensure scores file exists
    file.open("ZaxxonScores");
    file.close();

    // background must be done after player.
    pBackground = new Background(startStage, mainView, &spriteSheet, obstacles, enemies, *player, startPos, walls);
}


/// <summary>
/// Clean up memory when user is done playing the game.
/// </summary>
Game::~Game()
{
    const int obstaclesSize = obstacles.size();
    for (int i = 0; i < obstaclesSize; i++)
        delete obstacles[i];
    const int wallsSize = walls.size();
    for (int i = 0; i < wallsSize; i++)
        delete walls[i];
    delete pBackground;
}


/// <summary>
/// Start the game, main game loop is also managed in here.
/// </summary>
void Game::run() // if random erros later check that stack isnt full
{
    unsigned int fps;
    double deltaTime;
    std::chrono::steady_clock::time_point lastTime =
        std::chrono::high_resolution_clock::now(), currentTime;
    Background& background = *pBackground;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        /*else
        {
            //reset whenever boss is defeated
            //mainView.reset(sf::FloatRect(0, 0, 224, 224));
        }*/

        window.clear();

        if (gameState == 1)
        {
            window.setView(mainView);
            doCollision(player);

            // Fuel slowly runs out, player dies when fuel is empty.
            //if (fuelClock.getElapsedTime().asSeconds() >= 0.2 / gameSpeed)
            //{
            //    if (fuel-- == 0)
             //       playerDeath();
//
              //  fuelClock.restart();
           // }

            // Update window & objects
            background.update(window, mainView, gameSpeed, &spriteSheet, obstacles, enemies, *player, walls, bossState);
            for (unsigned int i = 0; i < obstacles.size(); i++)
                obstacles.at(i)->update(window);

            //Walls
            for (unsigned int i = 0; i < walls.size(); i++)
                walls.at(i)->drawWalls(window);

            for (Enemy* enemy : enemies)
                enemy->update(window, gameSpeed);

            player->update(window, background.getStage());

            window.setView(guiView);
            gui.render(window, player->getPos().y, score, highScore, fuel, lives);
        }
        else
        {
            window.setView(guiView);
            gui.startRender(window, highScore);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                gameState = 1;
        }

        window.display();

        // FPS
        currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = (std::chrono::duration_cast
            <std::chrono::nanoseconds>(currentTime - lastTime).count());
        lastTime = std::chrono::high_resolution_clock::now();

        fps = (unsigned int)(1000000000.0 / deltaTime);
        //std::cout << fps << "\n"; // temp but leave til done production
    }

    delete player;
    const int enemiesSize = enemies.size();
    for (int i = 0; i < enemiesSize; i++)
        delete enemies[i];
}


/// <summary>
/// Check collisions for all bullets and player.
/// </summary>
/// <param name="player"></param>
void Game::doCollision(Player* player)
{
    sf::Vector3f difference;

    //Turret Bullet Setup
    std::vector<sf::Vector3f> bulletPos;
    unsigned int size;

    //Plane Bullet Setup
    std::vector<sf::Vector3f> planeBulletPos;
    sf::Vector3f planePos;
    planePos = sf::Vector3f(player->getPos().x - 25, player->getPos().y, player->getPos().z - 20);

    for (unsigned int i = 0; i < obstacles.size(); i++)
    {
        if (!obstacles.at(i)->isPresent())
            continue;
        //Turret Bullets
        bulletPos = (obstacles.at(i)->getBulletLocations());

        for (unsigned int bullets = 0; bullets < bulletPos.size(); bullets++)
        {
            difference = sf::Vector3f(abs(bulletPos.at(bullets).x - planePos.x),
                abs(bulletPos.at(bullets).y - planePos.y),
                abs(bulletPos.at(bullets).z - planePos.z));

            if (difference.x < 15 && difference.y < 15 && difference.z < 15)
            {
                playerDeath();

                obstacles.at(i)->bulletKill(bullets);
            }
        }
        bulletPos = player->getBulletPosition();
        size = bulletPos.size();

        //Player Bullets Hitting Obstacles -- This only really works with translateTo2d 
        for (unsigned int pBullets = 0; pBullets < size; pBullets++)
        {
            difference = sf::Vector3f
            (abs(obstacles.at(i)->getPosition().x - bulletPos.at(pBullets).x),
                abs(obstacles.at(i)->getPosition().y - bulletPos.at(pBullets).y),
                abs(obstacles.at(i)->getPosition().z - bulletPos.at(pBullets).z));

            if (!(difference.x < 20 && difference.y < 20 && difference.z < 20))
                continue;
            obstacles.at(i)->kill();
            std::cout << "KILL";
            player->killBullet(pBullets);
            bulletPos.erase(bulletPos.begin() + pBullets);
            pBullets--;
            size--;

            //Scoring Swtich Statement
            switch (obstacles.at(i)->getType())
            {
            case 1:
                score += 300;
                fuel = 128;
                break;
            case 2:
                score += 1000;
                break;
            case 5:
                score += 150;
                break;
            case 6:
                score += 100;
                break;
            default:
                if (rand() % 1 == 0)
                    score += 200;
                else
                    score += 500;
                break;
            }

            if (score > highScore)
                highScore = score;
        }

        //Player Running into Obstacles
        difference = sf::Vector3f
        (abs(obstacles.at(i)->getPosition().x - planePos.x),
            abs(obstacles.at(i)->getPosition().y - planePos.y),
            abs(obstacles.at(i)->getPosition().z - planePos.z));

        if (difference.x < 20 && difference.y < 20 && difference.z < 10)
            playerDeath();
    }

    //TO DO CANNOT FIGURE THEM OUT
    //Wall Collisions
    for (unsigned int i = 0; i < walls.size(); i++)
    {
        if (!walls.at(i)->checkOnScreen())
            continue;

        //Player Running into Wall Sections We Placed
        for (unsigned int j = 0; j < walls.at(i)->getWallPositions().size(); j++)
        {
            //TO DO Fix it so it accounts for the position being top left
            difference = sf::Vector3f
            (abs(walls.at(i)->getWallPositions().at(j).x - 10 - planePos.x),
                abs(walls.at(i)->getWallPositions().at(j).y - 5 - planePos.y),
                abs(walls.at(i)->getWallPositions().at(j).z - planePos.z));

            if (difference.x < 20 && difference.y < 20 && difference.z < 10)
            {
                //std::cout << "Player Ran into wall" << std::endl;
                player->kill();

            }
        }

        //Plane runs into wall built into background
        difference.z = abs(planePos.z - walls.at(i)->getWallPositions().at(0).z);

        //TO DO fix it so the x works and the y plus value is more accurate
        if (planePos.y > (walls.at(i)->getWallPositions().at(0).y + 10) && difference.z < 30)
        {
            playerDeath();
        }
    }

    // Enemy bullets collision with player
    // Player bullets collision with enemy
    for (CharacterBullet& bullet : player->getBullets())
    {
        for (Enemy* enemy : enemies)
        {
            if (bullet.getSizeIndex() == enemy->getSizeIndex() &&
                bullet.getBounds().intersects(enemy->getBounds())
                )
            {
                bullet.kill();
                enemy->kill();
            }
        }
    }
}


void Game::playerDeath()
{
    player->kill();
    if (lives > 0)
        lives -= 1;
    else
    {
        gameState = 0;
        lives = 2;

        // We cannot append, eventually we overwrite scores as only top 6 will be shown
        std::string currentScores = "", line;
        file.open("ZaxxonScores", std::ios::in);
        if (file.is_open())
            while (std::getline(file, line))
                currentScores = currentScores + line;
        file.close();

        file.open("ZaxxonScores", std::ios::out);
        if (file.is_open())
            file << currentScores << "\n" << score;
        file.close();
    }

    // Enemy bullets collision with player
    // Player bullets collision with enemy
    for (Enemy* enemy : enemies)
    {
        // size index of bullets important
        if (enemy->getSizeIndex() == player->getSizeIndex())
        {

        }
    }

    fuel = 128;
    pBackground->resetPos(mainView, *player, 0);
}