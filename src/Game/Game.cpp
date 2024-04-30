#include "Game.h"

const unsigned int startPos = 0;
const Background::Stage startStage = Background::INITIAL;


/// <summary>
/// Initialize everything needed for the main game functionality.
/// </summary>
Game::Game()
	: window(sf::VideoMode(224, 256), "Zaxxon"), gui(&spriteSheet)
{
	// Seed the randomization system for enemies and score system
	srand((unsigned int)time(NULL));

	// Loading our sprites
	spriteSheet.loadFromFile("./res/spritesheet.png");
	bossSheet.loadFromFile("./res/ZaxxonFull.png");

	// Auto scaling, must be rounded to the nearest quarter to avoid visual bug
	float scale = (sf::VideoMode::getDesktopMode().height - 100) / 256.f;
	scale = round(scale * 4) / 4.f;

	window.setPosition(sf::Vector2i((int)(sf::VideoMode::getDesktopMode().width / 2.f - (224.f * scale) / 2.f), 0));
	//Set frame rate limit to smooth out
	window.setFramerateLimit(60);

	// Resize window to scale, resize everything else with it using view
	window.setSize(sf::Vector2u((unsigned int)(224.f * scale), (unsigned int)(256.f * scale)));
	mainView.reset(sf::FloatRect(0.f, 0.f, 224.f, 224.f));
	mainView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 224. / 256.));
	window.setView(mainView);

	// GUI must also be set at this time to overlay on the game screen
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
	pBackground = new Background(startStage, mainView, &spriteSheet, obstacles, enemies, *player, startPos, walls, zapWalls);

	file.open("ZaxxonScores", std::ios::in);
	if (file.is_open())
	{
		for (byte i = 0; i < 6; i++)
		{
			file >> currentScores[i];

			for (byte j = 0; j < 3; j++)
				file >> currentNames[i][j];
		}

		file.close();
	}

	highScore = currentScores[0];

	gui.renderScores(window, currentScores, currentNames);

	deathSprite.setTexture(spriteSheet);
	deathSprite.setTextureRect(sf::IntRect(80, 156, 19, 19));

	boss = new Boss(sf::Vector3f(-50, 139, -3800), player, &bossSheet, &spriteSheet);
	missile = new BossBullet(sf::Vector3f(0, 0, 1000), player, &spriteSheet); // spawn off screen
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

	const int enemiesSize = enemies.size();
	for (int i = 0; i < enemiesSize; i++)
		delete enemies[i];

	delete player;
	delete boss;
	delete pBackground;
	delete missile;
}


/// <summary>
/// Start the game, main game loop is also managed in here.
/// </summary>
void Game::run() // if random errors later check that stack isnt full
{
	Background& background = *pBackground;

	// Our game loop, will run forever until game is closed
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		// Each frame, clear the screen before drawing anything new
		window.clear();

		// State 1 is actively playing the game
		if (gameState == 1)
		{
			window.setView(mainView);

			// Things to do only when player is alive AKA these will be changed for player death
			if (player->isAlive())
			{
				// Fuel goes down every 0.2 seconds unless in space, then it goes down every 1.6 seconds
				bool inSpaceOffCD = background.isInSpace((int)player->getPos().z) && (fuelClock.getElapsedTime().asSeconds() >= 1.6 / gameSpeed);
				bool outSpaceOffCD = !background.isInSpace((int)player->getPos().z) && (fuelClock.getElapsedTime().asSeconds() >= 0.2 / gameSpeed);
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
				background.update(window, mainView, gameSpeed, &spriteSheet, obstacles, enemies, *player, walls, bossState, zapWalls);

				//Check if the boss has been defeated
				if (background.getStage() == 3 && boss->isDestroyed())
				{
					window.clear();
					window.setView(guiView);
					gui.renderWin(window);
					gui.render(window, player->getPos().y, score, highScore, fuel, lives);
					window.display();
					window.setView(mainView);

					sf::Clock tempClock;
					while (tempClock.getElapsedTime().asSeconds() < 5);

					fuel = 128, score += 1000;
					background.setStage(Background::INITIAL);
					background.resetPos(mainView, *player, 0);
					background.generateObstacles(Background::INITIAL, obstacles, &spriteSheet, walls, zapWalls);
					background.generateWaves(Background::INITIAL, enemies, &spriteSheet, (int)player->getPos().z);
					missile->setPos(sf::Vector3f(0, 0, 1000));

				}
			}
			else // Start the player death animation here
			{
				background.update(window, mainView, 0, &spriteSheet, obstacles, enemies, *player, walls, bossState, zapWalls);

				if (deathClock.getElapsedTime().asSeconds() > 1)
					// Make sure background is set back to default
					gameState = 2;
				else
				{
					// Flash background red for start of player death animation
					if (deathClock.getElapsedTime().asMilliseconds() / 200 % 2)
					{
						window.setView(guiView);
						pBackground->flashColor(window);
						window.setView(mainView);
					}
				}
			}

			// Update objects
			for (unsigned int i = 0; i < obstacles.size(); i++)
				obstacles.at(i)->update(window, (int)player->getPos().z);

			// Draw walls that are behind the player
			for (byte i = 0; i < walls.size(); i++) // For each wall...
				for (byte j = 0; j < walls.at(i)->getWallPositions().size() - 1; j++) // Then for each section in that wall...
					if (walls.at(i)->getWallPositions()[j].z < player->getPos().z) // Then if that wall z is more than player z...
						walls.at(i)->drawWalls(window); // Draw it behind player

			// Draw zap walls that are behind the player
			for (byte i = 0; i < zapWalls.size(); i++)
				if (zapWalls.at(i)->getStartPosition().z < player->getPos().z)
					zapWalls.at(i)->update(window);

			for (Enemy* enemy : enemies)
				enemy->update(window, gameSpeed);

			if (pBackground->getStage() == 3)
				boss->update(window);

			if (missile->isDestroyed())
			{
				delete missile;
				missile = new BossBullet(sf::Vector3f(0, 0, 1000), player, &spriteSheet); // spawn off screen
			}
			if (player->isMissileable() && abs(player->getPos().z - missile->getPos().z) > 1000)
				missile->setPos(player->getPos() + sf::Vector3f(0, 40, -400));
			missile->update(window);


			player->update(window, background.getStage(), gameSpeed);

			// Draw walls that are in front of the player
			for (byte i = 0; i < walls.size(); i++) // For each wall...
				for (byte j = 0; j < walls.at(i)->getWallPositions().size() - 1; j++) // Then for each section in that wall...
					if (walls.at(i)->getWallPositions()[j].z >= player->getPos().z) // Then if that wall z is less than player z...
						walls.at(i)->drawWalls(window); // Draw it in front of player

			// Draw zap walls that are in front of the player
			for (byte i = 0; i < zapWalls.size(); i++)
				if (zapWalls.at(i)->getStartPosition().z >= player->getPos().z)
					zapWalls.at(i)->update(window);

			window.setView(guiView);
			gui.render(window, player->getPos().y, score, highScore, fuel, lives);
		}
		else if (gameState == 0) // State 0 is main menu screen
		{
			window.setView(guiView);
			gui.startRender(window, highScore);

			if (zPressed())
				gameState = 1, score = 0;
		}
		else
		{
			window.setView(mainView);

			float time = deathClock.getElapsedTime().asSeconds();

			if (deathClock.getElapsedTime().asSeconds() < 2)
			{
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
			}
			else if (time >= 2 && lives > 0) // Reset pos backwards
			{
				player->kill();
				gameState = 1;

				// Not perfect but works (moved from player::kill() during death update)
				player->setPos(sf::Vector3f(0, 69, player->getPos().z));

				// You lose a life, this is not game over
				lives -= 1;

				// Prepare for respawn
				fuel = 128;
				if (pBackground->getStage() == Background::BOSSFIGHT)
					pBackground->setStage(Background::BOSS);
				pBackground->resetPos(mainView, *player, 0);
				pBackground->generateObstacles(pBackground->getStage(), obstacles, &spriteSheet, walls, zapWalls);
				pBackground->generateWaves(pBackground->getStage(), enemies, &spriteSheet, (int)player->getPos().z);
				missile->setPos(sf::Vector3f(0, 0, 1000));
			}
			else if (time < 5) // Show game over text
			{
				window.setView(guiView);
				gui.renderEnd(window);
			}
			else if (time < 25 && currentScores[5] < score) // Name entry
			{
				// Controls for the zaxxon keyboard
				if (upPressed() && activeCursor[0])
				{
					selector -= 10;

					if (selector > 200)
						selector -= 226;

					activeCursor[0] = false;
				}
				else if (!activeCursor[0] && !upPressed())
					activeCursor[0] = true;

				if (downPressed() && activeCursor[1])
				{
					selector += 10;

					if (selector > 29)
						selector -= 30;

					activeCursor[1] = false;
				}
				else if (!activeCursor[1] && !downPressed())
					activeCursor[1] = true;

				if (leftPressed() && activeCursor[2])
				{
					selector--;

					if (selector > 200)
						selector = 29;

					activeCursor[2] = false;
				}
				else if (!activeCursor[2] && !leftPressed())
					activeCursor[2] = true;

				if (rightPressed() && activeCursor[3])
				{
					selector++;

					if (selector > 29)
						selector = 0;

					activeCursor[3] = false;
				}
				else if (!activeCursor[3] && !rightPressed())
					activeCursor[3] = true;

				if (zPressed())
				{
					if (selector == 29) // END
						gameOver();
					else if (selector == 28) // RUB
					{
						for (byte i = 0; i < 3; i++)
							if (name[2 - i] != '_')
								name[2 - i] = '_', i = 3;
					}
					else // OTHERS
						for (byte i = 0; i < 3; i++)
						{
							if (name[i] == '_')
							{
								if (selector == 27)
									name[i] = ' ';
								else if (selector == 26)
									name[i] = 0;
								else
									name[i] = 'A' + selector;
								i = 3;
							}
						}
				}

				// Now render the keyboard and other name entry things
				window.setView(guiView);
				gui.renderEnd(window, (byte)(26 - time), selector, name);
			}
			else // Now actually game over
			{
				gameOver();
			}

			window.setView(guiView);
			gui.render(window, player->getPos().y, score, highScore, fuel, lives);
		}

		// Display everything we just drew to the screen
		window.display();

		// FPS
		currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = (std::chrono::duration_cast
			<std::chrono::nanoseconds>(currentTime - lastTime).count());
		lastTime = std::chrono::high_resolution_clock::now();

		fps = (unsigned int)(1000000000.0 / deltaTime);
		//std::cout << fps << "\n"; // temp but leave til done production
	}
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
	planePos = sf::Vector3f(player->getPos().x, player->getPos().y, player->getPos().z);

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
			score += obstacles.at(i)->getScore();
			switch (obstacles.at(i)->getType())
			{
			case 1:
				fuel = 128;
				break;
			case 7:
				fuel = 128;
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

		if (difference.x < 20 && difference.y < 15 && difference.z < 30)
			playerDeath();
	}

	// Wall Collisions
	for (unsigned int i = 0; i < walls.size(); i++)
	{
		// If this wall is not visible, we don't need to check its collision
		if (!walls.at(i)->checkOnScreen())
			continue;

		// Now check the sections for each wall if they have been hit or not
		for (unsigned int j = 0; j < walls.at(i)->getWallPositions().size(); j++)
		{
			//TO DO Fix it so it accounts for the position being top left
			difference = sf::Vector3f
			(abs(walls.at(i)->getWallPositions().at(j).x - planePos.x),
				abs(walls.at(i)->getWallPositions().at(j).y + 15 - planePos.y),
				abs(walls.at(i)->getWallPositions().at(j).z - planePos.z));

			if (difference.x < 15 && difference.y < 15 && difference.z < 10)
				playerDeath();
		}

		//Player runs into wall built into background
		difference.z = abs(planePos.z - walls.at(i)->getWallPositions().at(0).z);

		//TO DO fix it so the x works and the y plus value is more accurate
		if (planePos.y > (walls.at(i)->getWallPositions().at(0).y + 10) && difference.z < 20)
		{
			playerDeath();
		}

	}

	//Zap Walls Collisions
	for (unsigned int i = 0; i < zapWalls.size(); i++)
	{
		// If this zap wall is not visible, we don't need to check its collision
		if (!zapWalls.at(i)->isOnScreen())
			continue;

		difference = sf::Vector3f
		(abs(zapWalls.at(i)->getStartPosition().x - planePos.x),
			abs(zapWalls.at(i)->getStartPosition().y - planePos.y),
			abs(zapWalls.at(i)->getStartPosition().z - planePos.z));

		if (difference.y < 15 && difference.z < 15)
				playerDeath();
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
			bulletBounds.left += bulletBounds.width / 2.f - bulletSize / 2.f;
			bulletBounds.top += bulletBounds.height / 2.f - bulletSize / 2.f;
			bulletBounds.width = bulletSize;
			bulletBounds.height = bulletSize;

			if (bullet->getSizeIndex() == player->getSizeIndex() &&
				bulletBounds.intersects(modBounds)
				)
				playerDeath();
		}
	}

	BossBullet* bossMissile = boss->getMissile();
	int bulletNum = 0;

	// Player bullets collision with enemy
	for (CharacterBullet* bullet : player->getBullets())
	{
		sf::FloatRect bulletBounds = bullet->getBounds();
		bulletBounds.left += bulletBounds.width / 2.f - bulletSize / 2.f;
		bulletBounds.top += bulletBounds.height / 2.f - bulletSize / 2.f;
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
				player->drawHitmarker();
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
				(abs(walls.at(i)->getWallPositions().at(j).x - bullet->getPos().x),
					abs(walls.at(i)->getWallPositions().at(j).y + 15 - bullet->getPos().y),
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

		//Player Bullets hitting zap walls
		for (unsigned int i = 0; i < zapWalls.size(); i++)
		{
			if (!zapWalls.at(i)->isOnScreen())
				continue;

			difference = sf::Vector3f
			(abs(zapWalls.at(i)->getStartPosition().x - bullet->getPos().x),
				abs(zapWalls.at(i)->getStartPosition().y - bullet->getPos().y),
				abs(zapWalls.at(i)->getStartPosition().z - bullet->getPos().z));

			if (difference.y < 20 && difference.z < 20)
				bullet->kill(CharacterBullet::BulletDeathType::WallDeath);
		}

		//Player Bullets Hitting Boss
		if (abs(bullet->getPos().z - boss->getPos().z) <= 10 &&
			abs(bullet->getPos().x - boss->getPos().x) < 40 &&
			abs(bullet->getPos().y - boss->getPos().y) <= 40)
		{
			bullet->kill(CharacterBullet::BulletDeathType::WallDeath);

			//std::cout << bullet->getPos().x - boss->getPos().x << ", " << bullet->getPos().y - boss->getPos().y << std::endl;

			if (abs(bullet->getPos().z - boss->getPos().z) <= 10 &&
				abs(bullet->getPos().x - boss->getPos().x + 33) <= 10 &&
				abs(bullet->getPos().y - boss->getPos().y + 21) <= 10)
				boss->hit();
		}

		//Player bullets hitting boss missile
		if (boss->missileCreated() && abs(bullet->getPos().z - bossMissile->getPos().z) <= 10 &&
			abs(bullet->getPos().x - bossMissile->getPos().x - 50) < 20 &&
			abs(bullet->getPos().y - bossMissile->getPos().y) <= 20)
		{
			bossMissile->damage(2);
			bullet->kill(CharacterBullet::BulletDeathType::WallDeath);
		}

		//Player bullets hitting boss missile
		if (abs(bullet->getPos().z - missile->getPos().z) <= 10 &&
			abs(bullet->getPos().x - missile->getPos().x - 50) < 20 &&
			abs(bullet->getPos().y - missile->getPos().y) <= 20)
		{
			missile->damage(1);
			bullet->kill(CharacterBullet::BulletDeathType::WallDeath);
		}

		bulletNum++;
	}

	// boss missile
	if (boss->missileCreated() && abs(bossMissile->getPos().z - planePos.z) <= 5)
	{
		playerDeath();
		bossMissile->collide();
	}

	// game missile
	if (abs(missile->getPos().z - planePos.z) <= 5)
	{
		playerDeath();
		missile->collide();
	}
}


/// <summary>
/// Run code for when player dies.
/// </summary>
void Game::playerDeath()
{
	player->kill();
	// deathClock is used for player death anzimation, so start clock here.
	deathClock.restart();
}


/// <summary>
/// Run code for when you've fully ran out of lives.
/// </summary>
void Game::gameOver()
{
	gameState = 0;
	lives = 3;
	selector = 0;
	pBackground->setStage(Background::INITIAL);

	// Replace bottom score?
	if (currentScores[5] < score)
	{
		// When replacing and sorting, we need to keep the score and
		// initials for that score together for the leaderboard.
		currentScores[5] = score;
		currentNames[5] = name;

		// Now sort it
		for (int i = 0; i < 5; i++)
		{
			if (currentScores[5 - i] > currentScores[4 - i])
			{
				const int tempScore = currentScores[5 - i];
				currentScores[5 - i] = currentScores[4 - i];
				currentScores[4 - i] = tempScore;

				const std::string tempName = currentNames[5 - i];
				currentNames[5 - i] = currentNames[4 - i];
				currentNames[4 - i] = tempName;
			}
		}
	}

	file.open("ZaxxonScores", std::ios::out);
	for (byte i = 0; i < 6; i++)
	{
		file << currentScores[i] << ' ';

		for (byte j = 0; j < 3; j++)
			file << currentNames[i][j];

		file << ' ';
	}
	file.close();

	// Refresh leaderboard
	gui.renderScores(window, currentScores, currentNames);

	// Refresh name
	name[0] = '_', name[1] = '_', name[2] = '_';
}