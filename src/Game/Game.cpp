#include "Game.h"

const unsigned int startPos = 0;
const Background::Stage startStage = Background::INTRO;


/// <summary>
/// Initialize everything needed for the main game functionality.
/// </summary>
Game::Game()
	: window(sf::VideoMode(224, 256), "Zaxxon"), gui(&spriteSheet)
{
	// Seed the randomization system for enemies and score system
	srand(static_cast<unsigned int>(time(nullptr)));

	// Loading our sprites
	spriteSheet.loadFromFile("./res/spritesheet.png");
	bossSheet.loadFromFile("./res/ZaxxonFull.png");

	for (int file = 0; file < 5; file++)
	{
		std::string s = "./res/intro/image (" + std::to_string(file);
		s += ").png";
		intro[file].loadFromFile(s);

		if (file < 2)
			introLetters[file].setTexture(intro[file]);
		else if (file == 2)
		{
			introLetters[2].setTexture(intro[2]);
			introLetters[3].setTexture(intro[2]);
		}
		else
			introLetters[file + 1].setTexture(intro[file]);
	}

	introLetters[0].setPosition(sf::Vector2f(15.f, 90.f));
	introLetters[1].setPosition(sf::Vector2f(68.f, 90.f));
	introLetters[2].setPosition(sf::Vector2f(90.f, 90.f));
	introLetters[3].setPosition(sf::Vector2f(125.f, 90.f));
	introLetters[4].setPosition(sf::Vector2f(160.f, 90.f));
	introLetters[5].setPosition(sf::Vector2f(183.f, 90.f));

	introLetters[0].setScale(sf::Vector2f(.3f, .3f));
	introLetters[1].setScale(sf::Vector2f(.3f, .3f));
	introLetters[2].setScale(sf::Vector2f(.3f, .3f));
	introLetters[3].setScale(sf::Vector2f(.3f, .3f));
	introLetters[4].setScale(sf::Vector2f(.3f, .3f));
	introLetters[5].setScale(sf::Vector2f(.3f, .3f));

	introLetters[0].setTextureRect(sf::IntRect(0, 0, 0, 0));
	introLetters[1].setTextureRect(sf::IntRect(0, 0, 0, 0));
	introLetters[2].setTextureRect(sf::IntRect(0, 0, 0, 0));
	introLetters[3].setTextureRect(sf::IntRect(0, 0, 0, 0));
	introLetters[4].setTextureRect(sf::IntRect(0, 0, 0, 0));
	introLetters[5].setTextureRect(sf::IntRect(0, 0, 0, 0));

	// Auto scaling, must be rounded to the nearest quarter to avoid visual bug
	float scale = (sf::VideoMode::getDesktopMode().height - 100) / 256.f;
	scale = round(scale * 4) / 4.f;

	window.setPosition(sf::Vector2i(
		static_cast<int>(sf::VideoMode::getDesktopMode().width / 2.f - (224.f *
			scale) / 2.f), 0));
	//Set frame rate limit to smooth out
	window.setFramerateLimit(60);
	
	icon.loadFromFile("./res/Zaxxon_plane_cropped.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	// Resize window to scale, resize everything else with it using view
	window.setSize(sf::Vector2u(static_cast<unsigned int>(224.f * scale),
	                            static_cast<unsigned int>(256.f * scale)));
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
	pBackground = new Background(startStage, mainView, &spriteSheet, obstacles,
	                             enemies, *player, -300, walls, zapWalls);

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

	boss = new Boss(sf::Vector3f(-50, 139, -3800), player, &bossSheet,
	                &spriteSheet);
	missile = new BossBullet(sf::Vector3f(0, 0, 1000), player, &spriteSheet);
	// spawn off-screen
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

	const int zapWallsSize = zapWalls.size();
	for (int i = 0; i < zapWallsSize; i++)
		delete zapWalls[i];

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

			// Things to do only when player is alive AKA these 
			// will be changed for player death
			if (player->isAlive())
			{
				// Fuel goes down every 0.2 seconds unless in space, 
				// then it goes down every 1.6 seconds
				bool inSpaceOffCD = background.isInSpace(
						static_cast<int>(player->getPos().z)) &&
					(fuelClock.getElapsedTime().asSeconds() >= 1.6 / gameSpeed);
				bool outSpaceOffCD = !background.isInSpace(
						static_cast<int>(player->getPos().z)) &&
					(fuelClock.getElapsedTime().asSeconds() >= 0.2 / gameSpeed);

				// Collision checks
				doCollision(player);

				// Fuel slowly runs out, player dies when fuel is empty.
				if ((inSpaceOffCD || outSpaceOffCD) && background.getStage() !=
					3)
				{
					if (fuel-- == 0)
						playerDeath();

					fuelClock.restart();
				}
				// Move background
				background.update(window, mainView, gameSpeed, &spriteSheet,
				                  obstacles,
				                  enemies, *player, walls, zapWalls, reset);

				//Check if the boss has been defeated
				if (background.getStage() == 3 && boss->isDestroyed())
				{
					window.clear();
					window.setView(guiView);
					gui.renderWin(window);
					gui.render(window, player->getPos().y, player1data.score, player2data.score, highScore,
					           fuel, player2 ? player2data.lives : player1data.lives);
					window.display();
					window.setView(mainView);

					sf::Clock tempClock;
					while (tempClock.getElapsedTime().asSeconds() < 5);

					fuel = 128, player2 ? player2data.score += 1000 : player1data.score += 1000;
					reset++;

					//sets the background back to the initial stage
					background.setStage(Background::INITIAL);
					background.resetPos(mainView, *player, 0);
					background.generateObstacles(
						Background::INITIAL, obstacles, &spriteSheet, walls,
						zapWalls);
					background.generateWaves(Background::INITIAL, enemies,
					                         &spriteSheet,
					                         static_cast<int>(player->getPos().
						                         z));
					missile->setPos(sf::Vector3f(0, 0, 1000));
				}
			}
			else // Start the player death animation here
			{
				background.update(window, mainView, 0, &spriteSheet, obstacles,
				                  enemies,
				                  *player, walls, zapWalls, reset);

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

			// Draw obstacles that are behind the player
			for (unsigned int i = 0; i < obstacles.size(); i++)
				if (obstacles.at(i)->getPos().z < player->getPos().z)
					obstacles.at(i)->update(
						window, static_cast<int>(player->getPos().z), gameSpeed);

			// Draw walls that are behind the player
			for (byte i = 0; i < walls.size(); i++) // For each wall...
				for (byte j = 0; j < walls.at(i)->getWallPositions().size() - 1;
				     j++) // Then for each section in that wall...
					if (walls.at(i)->getWallPositions()[j].z < player->getPos().
						z) // Then if that wall z is more than player z...
						walls.at(i)->drawWalls(window); // Draw it behind player

			// Draw zap walls that are behind the player
			for (byte i = 0; i < zapWalls.size(); i++)
				if (zapWalls.at(i)->getStartPosition().z < player->getPos().z)
					zapWalls.at(i)->update(window, gameSpeed);

			// Draw enemies that are under the player
			for (Enemy* enemy : enemies)
				if (enemy->getPos().y > player->getPos().y)
					enemy->update(window, gameSpeed);

			if (pBackground->getStage() == 3)
				boss->update(window, gameSpeed);

			if (missile->isHit())
			{
				delete missile;
				missile = new BossBullet(sf::Vector3f(0, 0, 1000), player,
				                         &spriteSheet); // spawn off screen
			}
			if (player->isMissileable() && abs(
				player->getPos().z - missile->getPos().z) > 1000)
				missile->setPos(player->getPos() + sf::Vector3f(0, 40, -400));
			missile->update(window, gameSpeed);


			player->update(window, background.getStage(), gameSpeed);

			// Draw obstacles that are in front of the player
			for (unsigned int i = 0; i < obstacles.size(); i++)
				if (obstacles.at(i)->getPos().z >= player->getPos().z)
					obstacles.at(i)->update(
						window, static_cast<int>(player->getPos().z), gameSpeed);

			// Draw walls that are in front of the player
			for (byte i = 0; i < walls.size(); i++) // For each wall...
				for (byte j = 0; j < walls.at(i)->getWallPositions().size() - 1;
				     j++) // Then for each section in that wall...
					if (walls.at(i)->getWallPositions()[j].z >= player->getPos()
						.z) // Then if that wall z is less than player z...
						walls.at(i)->drawWalls(window);
			// Draw it in front of player

			// Draw zap walls that are in front of the player
			for (byte i = 0; i < zapWalls.size(); i++)
				if (zapWalls.at(i)->getStartPosition().z >= player->getPos().z)
					zapWalls.at(i)->update(window, gameSpeed);

			// Update lives
			PlayerData& data = player2 ? player2data : player1data;
			if (data.score > data.scoreThreshold)
			{
				if (lives < 4)
				{
					lives++;
					data.lives++;
				}
				data.scoreThreshold += 10000;
			}

			// Draw enemies that are above the player
			for (Enemy* enemy : enemies)
				if (enemy->getPos().y <= player->getPos().y)
					enemy->update(window, gameSpeed);
			// Draw GUI
			byte player2opt = player2mode;
			player2opt |= player2 << 1;

			window.setView(guiView);
			gui.render(window, player->getPos().y, player1data.score, player2data.score, highScore, fuel,
			           player2 ? player2data.lives : player1data.lives, player2opt);
		}
		else if (gameState == 4)
		{
			//do collisions -- only need bullet collisions
			doCollision(player);

			window.setView(mainView);

			// Move background
			background.update(window, mainView, gameSpeed, &spriteSheet, obstacles, 
				enemies, *player, walls, zapWalls, reset);

			//move player
			player->update(window, background.getStage(), gameSpeed);

			//draw walls, dont need to check z clipping because player should not reach
			for (unsigned int i = 0; i < walls.size(); i++)
				walls.at(i)->drawWalls(window);

			window.setView(guiView);
			gui.render(window, player->getPos().y, player1data.score, player2data.score, highScore, fuel, lives);

			//stops the game while the intro is displaying
			if (player->getPos().z <= 200)
			{
				gameSpeed = 0.0;

				doIntro();
			}
		}
		else if (gameState == 0) // State 0 is main menu screen
		{
			window.setView(guiView);
			gui.startRender(window, highScore);
			player->restartMissileTimer();
			player1data.score = 0;
			player2data.score = 0;

			if (onePressed())
			{
				player2mode = false;
				gameState = 1;
			}
			else if (twoPressed())
			{
				player2mode = true;
				gameState = 3;
				playerScreenTimer.restart();
			}
		}
		else if (gameState == 2) // dying
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
						deathSprite.move(-1 * (1 + i % 3) * time * 5,
						                 -3 * (1 + i % 3) * time * 5);
					else if (i < 6) // Left
						deathSprite.move(-3 * (1 + i % 3) * time * 5,
						                 1 * (1 + i % 3) * time * 5);
					else if (i < 9) // Below
						deathSprite.move(1 * (1 + i % 3) * time * 5,
						                 3 * (1 + i % 3) * time * 5);
					else // Right
						deathSprite.move(3 * (1 + i % 3) * time * 5,
						                 -1 * (1 + i % 3) * time * 5);

					window.draw(deathSprite);
				}
			}
			else if (time >= 2 && player2 ? player2data.lives > 0 : player1data.lives > 0) // Reset pos backwards
			{
				// You lose a life, this is not game over
				lives -= 1;
				if (player2)
					player2data.lives = lives;
				else
					player1data.lives = lives;

				player->kill();
				if (player2mode)
				{
					gameState = 3;
					playerScreenTimer.restart();
					player2 = !player2;
				}
				else
					gameState = 1;


				// Not perfect but works (moved from player::kill() during death update)
				player->setPos(sf::Vector3f(0, 69, player->getPos().z));

				// Prepare for respawn
				fuel = 128;
				pBackground->resetPos(mainView, *player, -500);
				pBackground->generateObstacles(pBackground->getStage(), obstacles, &spriteSheet, walls, zapWalls);
				pBackground->generateWaves(pBackground->getStage(), enemies, &spriteSheet, (int)player->getPos().z);
				if (pBackground->getStage() == Background::BOSS || pBackground->getStage() == Background::BOSSFIGHT)
					pBackground->setPosition(sf::Vector2f(0, 244));
				if (pBackground->getStage() == Background::BOSSFIGHT)
					pBackground->setStage(Background::BOSS);
				pBackground->resetPos(mainView, *player, 0);
				pBackground->generateObstacles(pBackground->getStage(),
				                               obstacles, &spriteSheet, walls,
				                               zapWalls);
				pBackground->generateWaves(pBackground->getStage(), enemies,
				                           &spriteSheet,
				                           static_cast<int>(player->getPos().
					                           z));
				missile->setPos(sf::Vector3f(0, 0, 1000));
				player->restartMissileTimer();
			}
			else if (time < 5) // Show game over text
			{
				window.setView(guiView);
				gui.renderEnd(window);
			}
			else if (!player2mode && time < 25 && currentScores[5] < player1data.score) // Name entry
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
				gui.renderEnd(window, static_cast<byte>(26 - time), selector,
				              name);
			}
			else // Now actually game over
				gameOver();

			window.setView(guiView);
			gui.render(window, player->getPos().y, player1data.score, player2data.score,
				highScore, fuel, player2 ? player2data.lives : player1data.lives);
		}
		else if (gameState == 3) // 2 player screen
		{
			byte player2opt = player2mode;
			player2opt |= player2 << 1;
			player2opt |= 1 << 2; // true << 2

			window.setView(guiView);
			gui.renderPlayerScreen(window, player2);
			gui.render(window, player->getPos().y, player1data.score, player2data.score,
				highScore, fuel, player2 ? player2data.lives : player1data.lives, player2opt);
			window.setView(mainView);

			if (playerScreenTimer.getElapsedTime().asSeconds() > 2)
				gameState = 1;
		}

		// Display everything we just drew to the screen
		window.display();
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
	planePos = sf::Vector3f(player->getPos().x, player->getPos().y,
	                        player->getPos().z);

	// Obstacle collisions
	for (unsigned int i = 0; i < obstacles.size(); i++)
	{
		Obstacle* obstacle = obstacles.at(i);
		if (!obstacle->isPresent())
			continue;

		//Turret Bullets
		bulletPos = (obstacle->getBulletLocations());

		for (unsigned int bullets = 0; bullets < bulletPos.size(); bullets++)
		{
			difference = sf::Vector3f(abs(bulletPos.at(bullets).x - planePos.x),
			                          abs(bulletPos.at(bullets).y - planePos.y),
			                          abs(bulletPos.at(
				                          bullets).z - planePos.z));

			if (difference.x < 15 && difference.y < 15 && difference.z < 25)
			{
				playerDeath();
				obstacle->bulletKill(bullets);
			}
		}

		bulletPos.clear();
		std::vector<CharacterBullet*>& bullets = player->getBullets();
		size = bullets.size();

		//Player Bullets Hitting Obstacles -- This only really works with translateTo2d 
		for (unsigned int bulletIndex = 0; bulletIndex < size; bulletIndex++)
		{
			difference = sf::Vector3f
			(abs(obstacle->getPos().x - bullets[bulletIndex]->getPos().x),
			 abs(obstacle->getPos().y - bullets[bulletIndex]->getPos().y),
			 abs(obstacle->getPos().z - bullets[bulletIndex]->getPos().z)
			);

			bool hit = obstacleHit(obstacle->getType(), difference,
			                       obstacle->getBounds().intersects(
				                       bullets[bulletIndex]->getBounds())
			);

			if (!hit)
				continue;
			obstacle->kill();
			player->killBullet(bulletIndex);
			bulletIndex--;
			size--;

			//Scoring Swtich Statement
			int score = player2 ? player2data.score : player1data.score;
			score += obstacle->getScore();
			switch (obstacle->getType())
			{
			case 1:
				fuel += 16;
				break;
			case 7:
				fuel += 16;
				break;
			}

			if (score > highScore)
				highScore = score;

			if (player2)
				player2data.score = score;
			else
				player1data.score = score;
		}

		//Player Running into Obstacles
		difference = sf::Vector3f
		(abs(obstacle->getPos().x - planePos.x),
		 abs(obstacle->getPos().y - planePos.y),
		 abs(obstacle->getPos().z - planePos.z)
		);
		bool hit = obstacleHit(obstacle->getType(), difference,
		                       player->getBounds().intersects(
			                       obstacle->getBounds())
		);

		if (hit)
			playerDeath();
	}

	// Wall Collisions
	for (unsigned int i = 0; i < walls.size(); i++)
	{
		// If this wall is not visible, we don't need to check its collision
		if (!walls.at(i)->checkOnScreen())
			continue;

		// Now check the sections for each wall if they have been hit or not
		for (unsigned int j = 0; j < walls.at(i)->getWallPositions().size(); j
		     ++)
		{
			difference = sf::Vector3f
			(abs(walls.at(i)->getWallPositions().at(j).x - 20 - (planePos.x-10)),
				abs(walls.at(i)->getWallPositions().at(j).y + 14 - planePos.y),
				abs(walls.at(i)->getWallPositions().at(j).z - 10 - planePos.z));

			if (difference.x < 25 && difference.y < 15 && difference.z < 10)
				playerDeath();
		}

		//Player runs into wall built into background
		difference.z =
			abs(planePos.z - walls.at(i)->getWallPositions().at(0).z);

		//TO DO fix it so the x works and the y plus value is more accurate
		if (planePos.y > (walls.at(i)->getWallPositions().at(0).y + 10) &&
			difference.z < 20)
			playerDeath();
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

		if (difference.y < 10 && difference.z < 10)
			playerDeath();
	}

	// Bounds can be changed here if want to change later.
	constexpr float hitboxSize = 4, bulletSize = 8;
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
				player2 ? player2data.score += 100 : player1data.score += 100;
			}
		}

		//Player Bullets hitting walls
		for (unsigned int i = 0; i < walls.size(); i++)
		{
			if (!walls.at(i)->checkOnScreen())
				continue;

			for (unsigned int j = 0; j < walls.at(i)->getWallPositions().size();
			     j++)
			{
				difference = sf::Vector3f
				(abs(walls.at(i)->getWallPositions().at(j).x - 20 - bullet->getPos().x),
					abs(walls.at(i)->getWallPositions().at(j).y + 15 - bullet->getPos().y),
					abs(walls.at(i)->getWallPositions().at(j).z - 10 - bullet->getPos().z));

				if (difference.x < 25 && difference.y < 20 && difference.z < 20)
					bullet->kill(CharacterBullet::BulletDeathType::WallDeath);
			}

			//Player Bullets hit walls build into background
			difference.z = abs(
				bullet->getPos().z - walls.at(i)->getWallPositions().at(0).z);

			if (planePos.y > (walls.at(i)->getWallPositions().at(0).y + 10) &&
				difference.z < 10)
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

			if (difference.y < 15 && difference.z < 15)
				bullet->kill(CharacterBullet::BulletDeathType::WallDeath);
		}

		//Player Bullets Hitting Boss
		if (abs(bullet->getPos().z - boss->getPos().z) <= 10 &&
			abs(bullet->getPos().x - boss->getPos().x) < 40 &&
			abs(bullet->getPos().y - boss->getPos().y) <= 40)
		{
			bullet->kill(CharacterBullet::BulletDeathType::WallDeath);

			if (abs(bullet->getPos().z - boss->getPos().z) <= 10 &&
				abs(bullet->getPos().x - boss->getPos().x + 33) <= 10 &&
				abs(bullet->getPos().y - boss->getPos().y + 21) <= 10)
				boss->hit();
		}

		//Player bullets hitting boss missile
		if (boss->missileCreated() && abs(
				bullet->getPos().z - bossMissile->getPos().z) <= 10 &&
			abs(bullet->getPos().x - bossMissile->getPos().x - 50) < 20 &&
			abs(bullet->getPos().y - bossMissile->getPos().y) <= 20)
		{
			bossMissile->damage(1);
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
	if (boss->missileCreated() && abs(bossMissile->getPos().z - planePos.z) <=
		5)
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
/// Returns whether the obstacle hit based on the difference and type.
/// </summary>
/// <param name="type"></param>
/// <param name="difference"></param>
/// <param name="intersect2d"></param>
/// <returns></returns>
bool Game::obstacleHit(Obstacle::ObstacleType type, sf::Vector3f difference,
                       bool intersect2d)
{
	bool hit = true;
	sf::Vector3f obstaclePos, playerPos;

	switch (type)
	{
	case Obstacle::GAS_CAN:
	case Obstacle::SATELLITE:
	case Obstacle::PLANE:
	case Obstacle::SPACE_FUEL:
	case Obstacle::GREEN_CANNON_RIGHT:
		hit = difference.x < 20 && difference.y < 15 && difference.z < 25;
		break;
	case Obstacle::GREY_CANNON:
	case Obstacle::GREEN_CANNON:
		hit = difference.x < 20 && difference.y < 5 && difference.z < 25;
		break;
	case Obstacle::MISSILE_UP:
		hit = intersect2d && difference.y < 8 && difference.z < 8;
		break;
	}

	return hit;
}


/// <summary>
/// Run code for when player dies.
/// </summary>
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
	lives = 3;
	player2data.lives = lives;
	player1data.lives = lives;
	selector = 0;
	pBackground->setStage(Background::INITIAL);
	player2 = false;
	player2mode = false;

	// Replace bottom score?
	if (!player2mode && currentScores[5] < player1data.score)
	{
		// When replacing and sorting, we need to keep the score and
		// initials for that score together for the leaderboard.
		currentScores[5] = player1data.score;
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


void Game::doIntro()
{
	//clock for frams as well as frame count
	static sf::Clock clock;
	static double prevTime = 0.0;
	static int frame = 0;

	//slows down frame rate
	if (clock.getElapsedTime().asMilliseconds() - prevTime >= 100.)
	{
		//differentiate each frame for each letter.
		if (frame < 17)
			introLetters[0].setTextureRect(sf::IntRect(643 * (frame % 2), 141 * (int)floor(frame / 2), 643, 141));
		else if (frame < 26 && frame > 18)
			introLetters[1].setTextureRect(sf::IntRect(57 * ((frame - 18) % 4), 105 * (int)floor((frame - 18) / 4), 57, 105));
		else if (frame < 36 && frame > 27)
			introLetters[2].setTextureRect(sf::IntRect(105 * ((frame - 27) % 3), 105 * (int)floor((frame - 27) / 3), 105, 105));
		else if (frame < 46 && frame > 37)
			introLetters[3].setTextureRect(sf::IntRect(105 * ((frame - 37) % 3), 105 * (int)floor((frame - 37) / 3), 105, 105));
		else if (frame < 55 && frame > 47)
			introLetters[4].setTextureRect(sf::IntRect(58 * ((frame - 47) % 4), 105 * (int)floor((frame - 47) / 4), 58, 105));
		else if (frame < 63 && frame > 56)
			introLetters[5].setTextureRect(sf::IntRect(81 * ((frame - 56) % 3), 105 * (int)floor((frame - 56) / 3), 81, 105));
		//reset to the main menu
		else if (frame > 80)
		{
			pBackground->changeStage(Background::INITIAL, mainView, &spriteSheet, obstacles,
				enemies, *player, startPos, walls, zapWalls);
			gameState = 0;

			gameSpeed = 1.2f;
		}

		frame++;

		prevTime = clock.getElapsedTime().asMilliseconds();
	}

	//draw
	for (int i = 0; i < 6; i++)
		if (introLetters[i].getTextureRect() != sf::IntRect(0, 0, 0, 0))
			window.draw(introLetters[i]);
}