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
	spriteSheet.loadFromFile("./res/spritesheet.png");
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

	file.open("ZaxxonScores", std::ios::in);
	if (file.is_open())
	{
		byte i = 0;

		while (!file.eof())
			file >> currentScores[i++];

		file.close();
	}

	highScore = currentScores[0];

	gui.renderScores(window, currentScores);

	deathSprite.setTexture(spriteSheet);
	deathSprite.setTextureRect(sf::IntRect(80, 156, 19, 19));
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

		window.clear();

		if (gameState == 1)
		{
			window.setView(mainView);

			// Things to do only when player is alive AKA these will be changed for player death
			if (player->isAlive())
			{
				// Fuel goes down every 0.2 seconds unless in space, then it goes down every 1.6 seconds
				bool inSpaceOffCD = background.isInSpace(player->getPos().z) && (fuelClock.getElapsedTime().asSeconds() >= 1.6 / gameSpeed);
				bool outSpaceOffCD = !background.isInSpace(player->getPos().z) && (fuelClock.getElapsedTime().asSeconds() >= 0.2 / gameSpeed);
				// Collision checks
				doCollision(player);

				// Fuel slowly runs out, player dies when fuel is empty.
				if ((inSpaceOffCD || outSpaceOffCD) && background.getStage() != 3)
				{
					if (fuel-- == 0)
						playerDeath();

					fuelClock.restart();
				}

				// Move background
				background.update(window, mainView, gameSpeed, &spriteSheet, obstacles, enemies, *player, walls, bossState);
			}
			else // Start the player death animation here
			{
				background.update(window, mainView, 0, &spriteSheet, obstacles, enemies, *player, walls, bossState);

				if (deathClock.getElapsedTime().asSeconds() > 1)
				{
					// Make sure background is set back to default
					gameState = 2;
					pBackground->flashColor(1);
				}
				else
				{
					// Flash background red for start of player death animation
					pBackground->flashColor(deathClock.getElapsedTime().asMilliseconds() / 200 % 2);
				}
			}

			// Update objects
			for (unsigned int i = 0; i < obstacles.size(); i++)
				obstacles.at(i)->update(window);

			// Draw Walls
			for (unsigned int i = 0; i < walls.size(); i++)
				walls.at(i)->drawWalls(window);

			for (Enemy* enemy : enemies)
				enemy->update(window, gameSpeed);

			player->update(window, background.getStage());

			window.setView(guiView);
			gui.render(window, player->getPos().y, score, highScore, fuel, lives);
		}
		else if (gameState == 0)
		{
			window.setView(guiView);
			gui.startRender(window, highScore);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
				gameState = 1, score = 0;
		}
		else
		{
			window.setView(mainView);

			// Change color of death explosion, 3 stages
			switch (deathClock.getElapsedTime().asMilliseconds() / 100 % 3)
			{
			case 0:
				deathSprite.setColor(sf::Color(255, 255, 255));
				break;
			case 1:
				deathSprite.setColor(sf::Color(222, 0, 0));
				break;
			case 2:
				deathSprite.setColor(sf::Color(0, 0, 0));
				break;
			}

			double time = deathClock.getElapsedTime().asSeconds();
			sf::Vector2f pos = player->getSpritePos();

			// Set position for each death explosion based on current time
			for (byte i = 0; i < 12; i++)
			{
				deathSprite.setPosition(pos);

				if (i < 3) // Above
					deathSprite.move(-1 * (1 + i % 3) * time * 5, -3 * (1 + i % 3) * time * 5);
				else if (i < 6) // Left
					deathSprite.move(-3 * (1 + i % 3) * time * 5, 1 * (1 + i % 3) * time * 5);
				else if (i < 9) // Below
					deathSprite.move(1 * (1 + i % 3) * time * 5, 3 * (1 + i % 3) * time * 5);
				else // Right
					deathSprite.move(3 * (1 + i % 3) * time * 5, -1 * (1 + i % 3) * time * 5);

				window.draw(deathSprite);
			}

			window.setView(guiView);
			gui.render(window, player->getPos().y, score, highScore, fuel, lives);

			if (deathClock.getElapsedTime().asSeconds() > 2) // Reset pos backwards
			{
				player->kill();
				gameState = 1;

				// Not perfect but works (moved from player::kill() during death update)
				player->setPos(sf::Vector3f(0, 69, player->getPos().z));

				// You lose a life, or game over if out of lives
				if (lives > 0)
					lives -= 1;
				else
					gameOver();

				// Prepare for respawn
				fuel = 128;
				pBackground->resetPos(mainView, *player, 0);
				pBackground->generateObstacles(pBackground->getStage(), obstacles, &spriteSheet, walls);
				pBackground->generateWaves(pBackground->getStage(), enemies, &spriteSheet, player->getPos().z);
				if (pBackground->getStage() == Background::BOSS || pBackground->getStage() == Background::BOSSFIGHT)
					pBackground->setPosition(sf::Vector2f(0, 244));
			}
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
	planePos = sf::Vector3f(player->getPos().x - 25, player->getPos().y, player->getPos().z);

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

			if (difference.x < 15 && difference.y < 15 && difference.z < 25)
			{
				playerDeath();
				obstacles.at(i)->bulletKill(bullets);
			}
		}

		bulletPos.clear();
		for (CharacterBullet* bullet : player->getBullets())
			bulletPos.push_back(bullet->getPos());
		size = bulletPos.size();

		//Player Bullets Hitting Obstacles -- This only really works with translateTo2d 
		for (unsigned int pBullets = 0; pBullets < size; pBullets++)
		{
			difference = sf::Vector3f
			(abs(obstacles.at(i)->getPosition().x - bulletPos.at(pBullets).x),
				abs(obstacles.at(i)->getPosition().y - bulletPos.at(pBullets).y),
				abs(obstacles.at(i)->getPosition().z - bulletPos.at(pBullets).z));

			if (!(difference.x < 20 && difference.y < 15 && difference.z < 25))
				continue;
			obstacles.at(i)->kill();
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
			case 7:
				score += 300;
				fuel = 128;
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

		if (difference.x < 30 && difference.y < 15 && difference.z < 30)
			playerDeath();
	}

	// Wall Collisions
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
				playerDeath();

			}
		}

		//Player runs into wall built into background
		difference.z = abs(planePos.z - walls.at(i)->getWallPositions().at(0).z);

		//TO DO fix it so the x works and the y plus value is more accurate
		if (planePos.y > (walls.at(i)->getWallPositions().at(0).y + 10) && difference.z < 10)
		{
			playerDeath();
		}
	}

	// Bounds can be changed here if want to change later.
	const float hitboxSize = 4, bulletSize = 8;
	sf::FloatRect modBounds;
	modBounds = player->getBounds();
	modBounds.left += modBounds.width / 2.f - hitboxSize / 2.f;
	modBounds.top += modBounds.height / 2.f - hitboxSize / 2.f;
	modBounds.width = hitboxSize;
	modBounds.height = hitboxSize;

	// Enemy bullets collision with player
	for (Enemy* enemy : enemies)
	{
		for (CharacterBullet* bullet : enemy->getBullets())
		{
			sf::FloatRect bulletBounds = bullet->getBounds();
			bulletBounds.left += bulletBounds.width / 2.f - bulletSize/2.f;
			bulletBounds.top += bulletBounds.height / 2.f - bulletSize/2.f;
			bulletBounds.width = bulletSize;
			bulletBounds.height = bulletSize;

			if (bullet->getSizeIndex() == player->getSizeIndex() &&
				bulletBounds.intersects(modBounds)
				)
				playerDeath();
		}
	}

	int bulletNum = 0;

	// Player bullets collision with enemy
	for (CharacterBullet* bullet : player->getBullets())
	{
		sf::FloatRect bulletBounds = bullet->getBounds();
		bulletBounds.left += bulletBounds.width / 2.f - bulletSize/2.f;
		bulletBounds.top += bulletBounds.height / 2.f - bulletSize/2.f;
		bulletBounds.width = bulletSize;
		bulletBounds.height = bulletSize;

		for (unsigned int i = 0; i < enemies.size(); i++)
		{
			Enemy* enemy = enemies[i];
			if (enemy->isDead())
				continue;
			modBounds = enemy->getBounds();
			modBounds.left += modBounds.width / 2 - hitboxSize / 2;
			modBounds.top += modBounds.height / 2 - hitboxSize / 2;
			modBounds.width = hitboxSize;
			modBounds.height = hitboxSize;

			if (bullet->getSizeIndex() == enemy->getSizeIndex() &&
				bulletBounds.intersects(modBounds)
				)
			{
				bullet->kill();
				enemy->kill();
			}
		}

		//Player Bullets hitting walls
		for (unsigned int i = 0; i < walls.size(); i++)
		{
			if (!walls.at(i)->checkOnScreen())
				continue;

			for (unsigned int j = 0; j < walls.at(i)->getWallPositions().size(); j++)
			{
				difference = sf::Vector3f
				(abs(walls.at(i)->getWallPositions().at(j).x - 10 - bullet->getPos().x),
					abs(walls.at(i)->getWallPositions().at(j).y - 5 - bullet->getPos().y),
					abs(walls.at(i)->getWallPositions().at(j).z - bullet->getPos().z));

				if (difference.x < 20 && difference.y < 20 && difference.z < 10)
					bullet->kill(CharacterBullet::BulletDeathType::WallDeath);
			}

			//Player Bullets hit walls build into background
			difference.z = abs(bullet->getPos().z - walls.at(i)->getWallPositions().at(0).z);

			//TO DO fix it so the x works and the y plus value is more accurate
			if (planePos.y > (walls.at(i)->getWallPositions().at(0).y + 10) && difference.z < 10)
				bullet->kill(CharacterBullet::BulletDeathType::WallDeath);
		}

		bulletNum++;
	}
}


void Game::playerDeath()
{
	player->kill();
	// deathClock is used for player death animation, so start clock here.
	deathClock.restart();
}


/// <summary>
/// Run code for when you've fully ran out of lives.
/// </summary>
void Game::gameOver()
{
	gameState = 0;
	lives = 2;
	pBackground->setStage(Background::INITIAL);

	// Replace bottom score?
	if (currentScores[5] < score)
	{
		currentScores[5] = score;

		// Now sort it
		for (int i = 0; i < 5; i++)
		{
			if (currentScores[5 - i] > currentScores[4 - i])
			{
				int temp = currentScores[5 - i];
				currentScores[5 - i] = currentScores[4 - i];
				currentScores[4 - i] = temp;
			}
		}
	}

	file.open("ZaxxonScores", std::ios::out);
	for (byte i = 0; i < 6; i++)
		file << currentScores[i] << ' ';
	file.close();
}