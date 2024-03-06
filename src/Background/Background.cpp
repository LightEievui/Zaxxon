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
	int startPos
)
{
	if (!initial.loadFromFile("res/BackgroundInitial.png"))
		std::cout << "Background file could not load\n";
	if (!space.loadFromFile("res/BackgroundSpace2.png"))
		std::cout << "Background file could not load\n";
	if (!boss.loadFromFile("res/BackgroundBoss.png"))
		std::cout << "Background file could not load\n";

	back.setTexture(initial);
	back.setOrigin(sf::Vector2f(0, back.getTexture()->getSize().y));
	back.setPosition(sf::Vector2f(0, 240));
	changeStage(startStage, mainView, spritesheet, obstacles, enemies, player, startPos);
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
	std::vector<Enemy*>& enemies, Player& player
)
{
	if(backgroundFinished(mainView))
	{
		if (stage == Stage::INITIAL)
		{
			stage = Stage::SPACE;
			back.setTexture(space);
		}
		else if (stage == Stage::SPACE)
		{
			stage = Stage::BOSS;
			back.setTexture(boss);
		}
		else
		{
			stage = Stage::INITIAL;
			back.setTexture(initial);
		}
		resetPos(mainView, player, 0);

		generateObstacles(stage, obstacles, spritesheet);
		generateWaves(stage, enemies, spritesheet, player.getPos().z);
	}
		//mainView.move(sf::Vector2f(.8f * gameSpeed, -.4f * gameSpeed));
	mainView.move(translateTo2d(sf::Vector3f(0, 0, -1.3 * gameSpeed)));//for translateTo2d

	// spawn waves that have gone past the z set in queue
	if (!waveQueue.empty() && player.getPos().z < waveQueue.front().first)
	{
		Enemy::spawnWave(enemies, spritesheet, player.getPos().z, waveQueue.front().second);
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
	int startPos
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

	generateObstacles(stage, obstacles, spritesheet);
	generateWaves(stage, enemies, spritesheet, player.getPos().z);
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
	back.setOrigin(sf::Vector2f(0, back.getTexture()->getSize().y));
	switch (stage)
	{
	case SPACE:
		back.setPosition(sf::Vector2f(0, 224));
		mainView.move(.8f * 350, -.4f * 350);
		player.resetPos(startPos + 350);
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
	std::vector<Obstacle*>& obstacles, sf::Texture* spriteSheet)
{
	/*Shooting Obstacles
	KEY
	0 = Grey Turrets
	1 = Green Turrets
	2 = Shooting Up Bullets
	
	Stationary Obstacles
	KEY
	1 = gas can
	2 = satellite
	3 = plane
	4 = closed end wall
	5 = open end wall
	*/

	obstacles.clear();
	switch (stage)
	{
	case INITIAL:
		//Shooting
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.6f, -380.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-15.f, 139.6f, -435.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.6f, -800.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.6f, -947.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-120.f, 139.6f, -1120.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.6f, -1145.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-142.f, 139.6f, -1410.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(0.f, 139.6f, -1680.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-180.f, 139.6f, -2335.f), spriteSheet, 100, 0));

		// Need to be Flipped - Green Turret
		obstacles.push_back(new Obstacle(sf::Vector3f(0.f, 139.6f, -2080.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(10.f, 139.6f, -2260.f), spriteSheet, 100, 1));

		//Shooting Up Missiles
		obstacles.push_back(new Obstacle(sf::Vector3f(-15.f, 139.6f, -435.f), spriteSheet, 100, 2));
		
		//Non-Shooting
		obstacles.push_back(new Obstacle(sf::Vector3f(-170.f, 139.6f, -340.f), spriteSheet, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-80.f, 139.6f, -590.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-12.f, 139.6f, -630.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-160.f, 139.6f, -665.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-170.f, 139.6f, -910.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-12.f, 139.6f, -1000.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-50.f, 139.6f, -1100.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-25.f, 139.6f, -1305.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-135.f, 139.6f, -1685.f), spriteSheet, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-40.f, 139.6f, -1770.f), spriteSheet, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-130.f, 139.6f, -1800.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-90.f, 139.6f, -1900.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-50.f, 139.6f, -2045.f), spriteSheet, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-95.f, 139.6f, -2175.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-40.f, 139.6f, -2320.f), spriteSheet, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-5.f, 139.6f, -2435.f), spriteSheet, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-45.f, 139.6f, -2460.f), spriteSheet, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-125.f, 139.6f, -2490.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 139.6f, -2635.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-150.f, 139.6f, -2635.f), spriteSheet, 2));

		//Walls
		obstacles.push_back(new Obstacle(sf::Vector3f(-2.f, 89.f, -138.f), spriteSheet, 4));
		obstacles.push_back(new Obstacle(sf::Vector3f(-172.f, 89.f, -140.f), spriteSheet, 5));

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
	enemies.clear();
	
	switch (stage)
	{
	case SPACE: // further below Z should be lesser
		waveQueue.push(std::pair<int, unsigned int>(-600, 0));
		waveQueue.push(std::pair<int, unsigned int>(-650, 0));
		waveQueue.push(std::pair<int, unsigned int>(-700, 0));
		waveQueue.push(std::pair<int, unsigned int>(-750, 0));

		break;
	}
}