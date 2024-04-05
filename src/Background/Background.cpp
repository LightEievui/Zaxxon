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
	int startPos, std::vector<Wall*>& walls
)
{
	if (!initial.loadFromFile("res/BackgroundInitial.png"))
		std::cout << "Background file could not load\n";
	if (!space.loadFromFile("res/BackgroundSpace2.png"))
		std::cout << "Background file could not load\n";
	if (!boss.loadFromFile("res/BackgroundBoss.png"))
		std::cout << "Background file could not load\n";

	back.setTexture(initial);
	back.setOrigin(sf::Vector2f(0, (float)back.getTexture()->getSize().y));
	back.setPosition(sf::Vector2f(0, 240));
	changeStage(startStage, mainView, spritesheet, obstacles, enemies, player, startPos, walls);
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
	std::vector<Enemy*>& enemies, Player& player, std::vector<Wall*>& walls, bool bossState
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

		generateObstacles(stage, obstacles, spritesheet, walls);
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
	int startPos, std::vector<Wall*>& walls
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

	generateObstacles(stage, obstacles, spritesheet, walls);
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

	if (!(stage == Stage::BOSS || stage == Stage::BOSSFIGHT))
		return wXPos >= 1830;
	//return wXPos >= back.getGlobalBounds().width; // temp
	else
		return wXPos >= 1300;
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
	switch (stage)
	{
	case SPACE:
		back.setPosition(sf::Vector2f(0, 224));
		mainView.move((int)(.8f * 350), (int)(-.4f * 350));
		player.resetPos(startPos + 350);
		break;
	case BOSS:
		back.setPosition(sf::Vector2f(0, 244));
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
	std::vector<Obstacle*>& obstacles, sf::Texture* spriteSheet, std::vector<Wall*>& walls)
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
		obstacles.push_back(new Obstacle(sf::Vector3f(0.f, 139.f, -1680.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(0.f, 139.f, -2080.f), spriteSheet, 100, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(10.f, 139.f, -2260.f), spriteSheet, 100, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.f, -2335.f), spriteSheet, 100, 0));

		//Shooting Up Missiles
		obstacles.push_back(new Obstacle(sf::Vector3f(-79.f, 139.f, -335.f), spriteSheet, 100, 2));

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

		/* Walls
		KEY for Vector
		0 = closed wall
		1 = open wall
		*/
		walls.push_back(new Wall(spriteSheet, sf::Vector3f(0.f, 70.f, -122.f), 2, std::vector<int> {0, 1}));
		walls.at(0)->setPosition(sf::Vector3f(-160.f, 70.f, -122.f), 1);
		walls.push_back(new Wall(spriteSheet, sf::Vector3f(-63.f, 130.f, -755.f), 3, std::vector<int> {1, 1, 1}));
		walls.push_back(new Wall(spriteSheet, sf::Vector3f(23.f, 130.f, -1423.f), 3, std::vector<int> {1, 1, 0}));

		//TO DO fix position
		walls.push_back(new Wall(spriteSheet, sf::Vector3f(-30.f, 130.f, -2790.f), 2, std::vector<int> {1, 1}));
		break;

	case SPACE:
		//TO DO adjust z position with plane waves / adjust angle of movement if desired

		//Blue Space Gas Cans
		obstacles.push_back(new Obstacle(sf::Vector3f(-120.f, 139.f, -960.f), spriteSheet, 4));
		obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 139.f, -2500.f), spriteSheet, 4));
		break;

	case BOSS:
		//TO DO add rest of obstacles and walls

		//First Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.f, -320.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.f, -370.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-120.f, 139.f, -460.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-30.f, 139.f, -460.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.f, -460.f), spriteSheet, 2));

		//Second Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.f, -800.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.f, -865.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.f, -920.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-30.f, 139.f, -910.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-140.f, 139.f, -910.f), spriteSheet, 100, 0));

		//Third Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-50.f, 139.f, -1200.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 139.f, -1210.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-140.f, 139.f, -1230.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-185.f, 139.f, -1275.f), spriteSheet, 100, 1));

		//Fourth Area
		obstacles.push_back(new Obstacle(sf::Vector3f(-40.f, 139.f, -1510.f), spriteSheet, 100, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-200.f, 139.f, -1540.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-30.f, 139.f, -1630.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.f, -1670.f), spriteSheet, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-80.f, 139.f, -1680.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-130.f, 139.f, -1750.f), spriteSheet, 100, 0));

		//Fifth Area
		//TO DO fix positions
		obstacles.push_back(new Obstacle(sf::Vector3f(-40.f, 139.f, -1960.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-80.f, 139.f, -2090.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-50.f, 139.f, -2160.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-70.f, 139.f, -2170.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-110.f, 139.f, -2170.f), spriteSheet, 1));

		//Sixth Area
		//TO DO

		//Seventh Area
		//TO DO

		//Eight Area
		//TO DO 

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
	std::queue<std::pair<int, unsigned int>>().swap(waveQueue); // waveQueue.clear();

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
		waveQueue.push(std::pair<int, unsigned int>(-790, 3)); // HERE set to 2/3 difference
		waveQueue.push(std::pair<int, unsigned int>(-1260, 4));
		waveQueue.push(std::pair<int, unsigned int>(-1350, 4));
		waveQueue.push(std::pair<int, unsigned int>(-1400, 4));
		waveQueue.push(std::pair<int, unsigned int>(-1460, 5));
		waveQueue.push(std::pair<int, unsigned int>(-1560, 6));
		waveQueue.push(std::pair<int, unsigned int>(-1610, 6));
		waveQueue.push(std::pair<int, unsigned int>(-1660, 6));
		waveQueue.push(std::pair<int, unsigned int>(-1710, 6));


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
void Background::flashColor(int state)
{
	if (state)
		back.setColor(sf::Color(255, 255, 255));
	else
		back.setColor(sf::Color(222, 100, 100));
}