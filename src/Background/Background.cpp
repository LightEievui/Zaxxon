#include "Background.h"
#include "Util/Util.h"


/// <summary>
/// Create the background object and initialize each of the necessary images
/// so they are ready to be drawn when needed.
/// </summary>
/// <param name="startStage"></param>
/// <param name="mainView"></param>
/// <param name="spritesheet"></param>
/// <param name="obstacles"></param>
/// <param name="enemies"></param>
/// <param name="player"></param>
/// <param name="startPos"></param>
Background::Background(Stage startStage, sf::View& mainView, sf::Texture* spritesheet,
	std::vector<Obstacle*>& obstacles, std::vector<Enemy*>& enemies, Player& player,
	int startPos, std::vector<Wall*>& walls, std::vector <ZapWall*>& zapWalls
)
{
	if (!initial.loadFromFile("res/BackgroundInitial.png"))
		std::cout << "Background file could not load\n";
	if (!space.loadFromFile("res/BackgroundSpace2.png"))
		std::cout << "Background file could not load\n";
	if (!boss.loadFromFile("res/BackgroundBoss.png"))
		std::cout << "Background file could not load\n";

	back.setTexture(boss);
	back.setOrigin(sf::Vector2f(0, (float)back.getTexture()->getSize().y));
	back.setPosition(sf::Vector2f(0, 240));
	changeStage(startStage, mainView, spritesheet, obstacles, enemies, player, 
		startPos, walls, zapWalls);

	if (!death.loadFromFile("res/BackgroundDeath.png"))
		std::cout << "Death overlay file failed to load\n";

	deathOverlay.setTexture(death);
	deathOverlay.setColor(sf::Color(255, 255, 255, 100));
}


Background::~Background()
{

}


/// <summary>
/// Run the background logic, and check if background stage should be swapped.
/// </summary>
/// <param name="window"></param>
/// <param name="mainView"></param>
/// <param name="gameSpeed"></param>
/// <param name="spritesheet"></param>
/// <param name="obstacles"></param>
/// <param name="enemies"></param>
/// <param name="player"></param>
void Background::update(sf::RenderWindow& window, sf::View& mainView,
	float gameSpeed, sf::Texture* spritesheet, std::vector<Obstacle*>& obstacles,
	std::vector<Enemy*>& enemies, Player& player, std::vector<Wall*>& walls, bool bossState, std::vector <ZapWall*>& zapWalls
)
{
	if (backgroundFinished(mainView))
	{
		if (stage == Stage::INITIAL)
		{
			stage = Stage::SPACE;
			back.setTexture(space);
			resetPos(mainView, player, 0);
		}
		else if (stage == Stage::SPACE)
		{
			stage = Stage::BOSS;
			back.setTexture(boss);
			resetPos(mainView, player, 0);
		}
		else if (stage == Stage::BOSS)
		{
			stage = Stage::BOSSFIGHT;
		}
		else if (!bossState)
		{
			stage = Stage::INITIAL;
			back.setTexture(initial);
			resetPos(mainView, player, 0);
		}


		//if (stage == Stage::BOSS)
		//{
		//	stage = Stage::BOSSFIGHT;
		//}

		generateObstacles(stage, obstacles, spritesheet, walls, zapWalls);
		generateWaves(stage, enemies, spritesheet, (int)player.getPos().z);
	}

	if (!backgroundFinished(mainView))
	{
		//mainView.move(sf::Vector2f(.8f * gameSpeed, -.4f * gameSpeed));
		float modifier = 1;
		if (stage == SPACE) // space moves 2/3 speed
			modifier = 0.66f;
		mainView.move(translateTo2d(sf::Vector3f(0, 0, -1.3f * gameSpeed * modifier)));//for translateTo2d
	}

	// spawn waves that have gone past the z set in queue
	if (!waveQueue.empty() && player.getPos().z < waveQueue.front().first)
	{
		Enemy::spawnWave(enemies, spritesheet, (int)player.getPos().z, waveQueue.front().second);
		waveQueue.pop();
	}

	window.draw(back);
}


/// <summary>
/// Set the position of the background.
/// </summary>
/// <param name="pos"></param>
void Background::setPosition(sf::Vector2f pos)
{
	back.setPosition(pos);
}


/// <summary>
/// Change what stage the background is on, needed for the background image
/// and generate functions.
/// </summary>
/// <param name="stage"></param>
/// <param name="mainView"></param>
/// <param name="spritesheet"></param>
/// <param name="obstacles"></param>
/// <param name="enemies"></param>
/// <param name="player"></param>
/// <param name="startPos"></param>
void Background::changeStage(Stage stage, sf::View& mainView, sf::Texture* spritesheet,
	std::vector<Obstacle*>& obstacles, std::vector<Enemy*>& enemies, Player& player,
	int startPos, std::vector<Wall*>& walls, std::vector <ZapWall*>& zapWalls
)
{
	this->stage = stage;
	if (stage == Stage::INITIAL)
		back.setTexture(initial);
	else if (stage == Stage::SPACE)
		back.setTexture(space);
	else
		back.setTexture(boss);
	resetPos(mainView, player, startPos);

	generateObstacles(stage, obstacles, spritesheet, walls, zapWalls);
	generateWaves(stage, enemies, spritesheet, (int)player.getPos().z);
}


/// <summary>
/// Check if current background stage is finished.
/// </summary>
/// <param name="view"></param>
/// <returns>A boolean</returns>
bool Background::backgroundFinished(sf::View& view)
{
	//float wXPos = view.getCenter().x + (view.getSize().x / 2); // temp
	float wXPos = view.getCenter().x - (view.getSize().x / 2);

	if (stage == Stage::BOSS)
		return wXPos >= 2050;
	else if (stage == Stage::SPACE)
		return wXPos >= 1150;
	else
		return wXPos >= 1830;
	//return wXPos >= back.getGlobalBounds().width; // temp
}


/// <summary>
/// Check if background is currently space, this affects the player.
/// </summary>
/// <param name="z"></param>
/// <returns>A boolean</returns>
bool Background::isInSpace(int z)
{
	// Initial -2800: space
	// Boss -123: No space
	bool inSpace = false;
	switch (stage)
	{
	case INITIAL:
		if (z > -123 || z < -2800)
			inSpace = true;
		break;
	case SPACE:
		inSpace = true;
		break;
	case BOSS:
		if (z > -123)
			inSpace = true;
		break;
	}

	return inSpace;
}


/// <summary>
/// Reset the position of the background for stage changes.
/// </summary>
/// <param name="mainView"></param>
/// <param name="player"></param>
/// <param name="startPos"></param>
void Background::resetPos(sf::View& mainView, Player& player, int startPos)
{
	//Sets the origin to the bottom left corner as that is where it will start 
	//on the screen
	mainView.setCenter(sf::Vector2f(112, 100));
	back.setOrigin(sf::Vector2f(0, (float)back.getTexture()->getSize().y));

	int adder = stage == SPACE ? 350 : 0;
	sf::Vector2f moveVector = sf::Vector2f(.8f * (startPos + adder), -.4f * (startPos + adder));
	mainView.move(moveVector);

	switch (stage)
	{
	case SPACE:
		back.setPosition(sf::Vector2f(0, 224));
		player.resetPos(startPos + 350);
		break;
	case BOSS:
		back.setPosition(sf::Vector2f(0, 238));
		player.resetPos(startPos);
		break;
	default:
		back.setPosition(sf::Vector2f(0, 224));
		player.resetPos(startPos);
	}
}


/// <summary>
/// Place all obstacles for given stage.
/// </summary>
/// <param name="stage"></param>
/// <param name="obstacles"></param>
/// <param name="spriteSheet"></param>
void Background::generateObstacles(Background::Stage stage,
	std::vector<Obstacle*>& obstacles, sf::Texture* spriteSheet, std::vector<Wall*>& walls, std::vector <ZapWall*>& zapWalls)
{
	/*Shooting Obstacles
	KEY
	0 = Grey Turrets
	1 = Green Turrets
	2 = Shooting Up Bullets
	3 = Green Shooting Right

	Stationary Obstacles
	KEY
	1 = gas can
	2 = satellite
	3 = plane
	4 = blue floating gas can
	*/

	for (Obstacle* obstacle : obstacles)
		delete obstacle;
	obstacles.clear();

	for (Wall* wall : walls)
		delete wall;
	walls.clear();

	for (ZapWall* zapWall : zapWalls)
		delete zapWall;
	zapWalls.clear();

	switch (stage)
	{
	case INITIAL:
		//Shooting
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.f, -380.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-15.f, 139.f, -435.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.f, -800.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.f, -947.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-120.f, 139.f, -1120.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.f, -1145.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-142.f, 139.f, -1410.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-10.f, 139.f, -1680.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-5.f, 139.f, -2080.f), spriteSheet, 100, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-5.f, 139.f, -2260.f), spriteSheet, 100, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.f, -2335.f), spriteSheet, 100, 0));


		//Non-Shooting
		obstacles.push_back(new Obstacle(sf::Vector3f(-170.f, 139.f, -340.f), spriteSheet, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-80.f, 139.f, -590.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-12.f, 139.f, -630.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-160.f, 139.f, -665.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-170.f, 139.f, -910.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-12.f, 139.f, -1000.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-50.f, 139.f, -1100.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-25.f, 139.f, -1305.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-135.f, 139.f, -1685.f), spriteSheet, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-40.f, 139.f, -1770.f), spriteSheet, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-130.f, 139.f, -1800.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.f, -1900.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-50.f, 139.f, -2045.f), spriteSheet, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-95.f, 139.f, -2175.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-40.f, 139.f, -2320.f), spriteSheet, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-5.f, 139.f, -2435.f), spriteSheet, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-45.f, 139.f, -2460.f), spriteSheet, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-125.f, 139.f, -2490.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 139.f, -2635.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-150.f, 139.f, -2635.f), spriteSheet, 2));

		//Shooting Up Missiles
		obstacles.push_back(new Obstacle(sf::Vector3f(-79.f, 139.f, -335.f), spriteSheet, 130, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-25.f, 139.f, -534.298f), spriteSheet, 300, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-176.f, 139.6f, -550.697f), spriteSheet, 370, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-15.f, 139.6f, -857.029f), spriteSheet, 650, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-135.f, 139.f, -840.029f), spriteSheet, 650, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-62.f, 139.f, -1048.13f), spriteSheet, 850, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-141.f, 139.f, -1218.6f), spriteSheet, 950, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-10.f, 139.f, -1258.41f), spriteSheet, 1050, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-30.f, 139.f, -1515.31f), spriteSheet, 1350, 4));
		obstacles.push_back(new Obstacle(sf::Vector3f(-185.f, 139.f, -1460.61f), spriteSheet, 1350, 4));
		obstacles.push_back(new Obstacle(sf::Vector3f(-145.f, 139.f, -1644.61f), spriteSheet, 1450, 4));
		obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 139.333f, -1764.57f), spriteSheet, 1700, 4));
		obstacles.push_back(new Obstacle(sf::Vector3f(-98.4f, 139.f, -2115.49f), spriteSheet, 1850, 4));
		obstacles.push_back(new Obstacle(sf::Vector3f(-20.2f, 139.333f, -2148.29f), spriteSheet, 1850, 4));
		obstacles.push_back(new Obstacle(sf::Vector3f(-151.2f, 139.f, -2268.38f), spriteSheet, 2100, 4));
		obstacles.push_back(new Obstacle(sf::Vector3f(-65.2f, 139.667f, -2257.88f), spriteSheet, 2100, 4));
		obstacles.push_back(new Obstacle(sf::Vector3f(-42.6f, 139.f, -2617.49f), spriteSheet, 2300, 4));
		// -4.6f, 139.f, -2628.49f

		/* Walls
		KEY for Vector
		0 = closed wall
		1 = open wall
		*/
		walls.push_back(new Wall(spriteSheet, sf::Vector3f(0.f, 70.f, -122.f), 2, std::vector<int> {0, 1}));
		walls.at(0)->setPosition(sf::Vector3f(-160.f, 70.f, -122.f), 1);
		walls.push_back(new Wall(spriteSheet, sf::Vector3f(-63.f, 130.f, -755.f), 3, std::vector<int> {1, 1, 1}));
		walls.push_back(new Wall(spriteSheet, sf::Vector3f(23.f, 130.f, -1423.f), 3, std::vector<int> {1, 1, 0}));
		walls.push_back(new Wall(spriteSheet, sf::Vector3f(-26.f, 130.f, -2782.f), 2, std::vector<int> {1, 1}));

		//Zap Walls
		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(-10, 125, -1940)));
		break;

	case SPACE:
		//TO DO adjust z position with plane waves / adjust angle of movement if desired

		//Blue Space Gas Cans
		obstacles.push_back(new Obstacle(sf::Vector3f(-120.f, 139.f, -960.f), spriteSheet, 4));
		obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 139.f, -2500.f), spriteSheet, 4));
		break;

	case BOSS:
		//Area are made by the sections of map between walls

		walls.push_back(new Wall(spriteSheet, sf::Vector3f(-9.f, 70.f, -106.f), 3, std::vector<int> {0, 1, 1}));
		walls.at(0)->setPosition(sf::Vector3f(-130.f, 70.f, -106.f), 1);
		walls.at(0)->setPosition(sf::Vector3f(-168.f, 70.f, -106.f), 2);
		
		//First Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.f, -320.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.f, -370.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-120.f, 139.f, -460.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-30.f, 139.f, -460.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.f, -460.f), spriteSheet, 2));

		walls.push_back(new Wall(spriteSheet, sf::Vector3f(7.f, 112.f, -550.f), 2, std::vector<int> {0, 1}));
		walls.at(1)->setPosition(sf::Vector3f(-153.f, 112.f, -550.f), 1);

		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(-10, 90, -550)));
		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(-10, 70, -550)));

		//Second Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.f, -800.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.f, -865.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.f, -920.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-30.f, 139.f, -910.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-140.f, 139.f, -910.f), spriteSheet, 100, 0));

		walls.push_back(new Wall(spriteSheet, sf::Vector3f(-136.f, 82.f, -978.f), 2, std::vector<int> {0, 1}));

		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(-30, 60, -978)));

		//Third Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-50.f, 139.f, -1200.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 139.f, -1210.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-140.f, 139.f, -1230.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-185.f, 139.f, -1275.f), spriteSheet, 100, 1));

		walls.push_back(new Wall(spriteSheet, sf::Vector3f(27.f, 139.f, -1408.f), 3, std::vector<int> {1, 0, 1}));
		walls.at(3)->setPosition(sf::Vector3f(-133.f, 139.f, -1407.f), 2);

		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(0, 120, -1407)));
		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(0, 100, -1407)));
		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(0, 80, -1407)));

		//Fourth Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-40.f, 139.f, -1510.f), spriteSheet, 100, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-200.f, 139.f, -1540.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-30.f, 139.f, -1630.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.f, -1670.f), spriteSheet, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-80.f, 139.f, -1680.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-130.f, 139.f, -1750.f), spriteSheet, 100, 0));

		walls.push_back(new Wall(spriteSheet, sf::Vector3f(10.f, 112.f, -1830.f), 3, std::vector<int> {0, 1, 1}));
		walls.at(4)->setPosition(sf::Vector3f(-112.f, 112.f, -1829.f), 1);
		walls.at(4)->setPosition(sf::Vector3f(-152.f, 112.f, -1829.f), 2);

		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(0, 90, -1829)));
		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(0, 70, -1829)));

		//Fifth Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-35.f, 139.f, -1990.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-110.f, 139.f, -2110.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-50.f, 139.f, -2180.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 139.f, -2180.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-160.f, 139.f, -2180.f), spriteSheet, 1));

		walls.push_back(new Wall(spriteSheet, sf::Vector3f(-15.f, 82.f, -2260.f), 3, std::vector<int> {0, 1, 1}));
		walls.at(5)->setPosition(sf::Vector3f(-133.f, 82.f, -2259.f), 1);
		walls.at(5)->setPosition(sf::Vector3f(-173.f, 82.f, -2259.f), 2);

		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(-20, 60, -2259)));
		
		//Sixth Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-40.f, 139.f, -2420.f), spriteSheet, 100, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-105.f, 139.f, -2540.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-45.f, 139.f, -2580.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-155.f, 139.f, -2580.f), spriteSheet, 2));

		walls.push_back(new Wall(spriteSheet, sf::Vector3f(-56.f, 139.f, -2687.f), 3, std::vector<int> {1, 1, 1}));
	
		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(0, 120, -2687)));
		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(0, 100, -2687)));
		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(0, 80, -2687)));
		
		//Seventh Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-55.f, 139.f, -2820.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-60.f, 139.f, -2875.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-50.f, 139.f, -2950.f), spriteSheet, 100,  3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-50.f, 139.f, -3010.f), spriteSheet, 1));

		walls.push_back(new Wall(spriteSheet, sf::Vector3f(-40.f, 82.f, -3060.f), 3, std::vector<int> {0, 1, 1}));
		walls.at(7)->setPosition(sf::Vector3f(-161.f, 82.f, -3060.f), 1);
		walls.at(7)->setPosition(sf::Vector3f(-201.f, 82.f, -3060.f), 2);
		
		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(-50, 60, -3060)));
		zapWalls.push_back(new ZapWall(spriteSheet, sf::Vector3f(-50, 40, -3060)));

		//Eight Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-35.f, 139.f, -3240.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-80.f, 139.f, -3250.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-35.f, 139.f, -3330.f), spriteSheet, 1));

		for (Obstacle* obstacle : obstacles)
			obstacle->setPos(obstacle->getPos() + sf::Vector3f(0, -6, 0));
		for (ZapWall* zw : zapWalls)
			zw->setStartPos(zw->getStartPosition() + sf::Vector3f(0, -6, 0));
		for (Wall* wall : walls)
			for(int i = 0; i < wall->getWallPositions().size(); i++)
				wall->setPosition(wall->getWallPositions()[i] + sf::Vector3f(0, -6, 0), i);
		break;
	}
}


/// <summary>
/// Handle enemy wave generation.
/// </summary>
/// <param name="stage"></param>
/// <param name="enemies"></param>
/// <param name="spriteSheet"></param>
/// <param name="playerZ"></param>
void Background::generateWaves(Background::Stage stage,
	std::vector<Enemy*>& enemies, sf::Texture* spriteSheet, int playerZ)
{
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();
	// equivalent to waveQueue.clear();
	std::queue<std::pair<int, unsigned int>>().swap(waveQueue);

	// format waveQueue.push(std::pair<int, unsigned int>(spawnZ, waveId));

	switch (stage)
	{
	case SPACE: // further below Z should be lesser
		waveQueue.push(std::pair<int, unsigned int>(-460, 0));
		waveQueue.push(std::pair<int, unsigned int>(-493, 0));
		waveQueue.push(std::pair<int, unsigned int>(-526, 0));
		waveQueue.push(std::pair<int, unsigned int>(-559, 0));
		waveQueue.push(std::pair<int, unsigned int>(-652, 1));
		waveQueue.push(std::pair<int, unsigned int>(-665, 2));
		waveQueue.push(std::pair<int, unsigned int>(-678, 3));
		waveQueue.push(std::pair<int, unsigned int>(-991, 4));
		waveQueue.push(std::pair<int, unsigned int>(-1051, 4));
		waveQueue.push(std::pair<int, unsigned int>(-1084, 4));
		waveQueue.push(std::pair<int, unsigned int>(-1104, 5));
		waveQueue.push(std::pair<int, unsigned int>(-1170, 6));
		waveQueue.push(std::pair<int, unsigned int>(-1203, 6));
		waveQueue.push(std::pair<int, unsigned int>(-1236, 6));
		waveQueue.push(std::pair<int, unsigned int>(-1269, 6));
		waveQueue.push(std::pair<int, unsigned int>(-1315, 7));
		waveQueue.push(std::pair<int, unsigned int>(-1350, 3));
		waveQueue.push(std::pair<int, unsigned int>(-1400, 2));
		waveQueue.push(std::pair<int, unsigned int>(-1440, 4));
		waveQueue.push(std::pair<int, unsigned int>(-1490, 4));


		break;
	}
}


/// <summary>
/// Get the current stage background is on.
/// </summary>
/// <returns>A background stage</returns>
Background::Stage Background::getStage()
{
	return stage;
}


/// <summary>
/// Flash color of background between the default color and red tinted color
/// based on state variable passed into the method. 0 is red, 1 is default.
/// This is used for the first part of the death animation in Game.cpp
/// </summary>
/// <param name="state"></param>
void Background::flashColor(sf::RenderWindow& window)
{
	window.draw(deathOverlay);
}


/// <summary>
/// Set the current stage
/// </summary>
/// <param name="newStage"></param>
void Background::setStage(Stage newStage)
{
	stage = newStage;

	if (stage == INITIAL)
		back.setTexture(initial);
	else if (stage == SPACE)
		back.setTexture(space);
	else if (stage == BOSS)
		back.setTexture(boss);
}