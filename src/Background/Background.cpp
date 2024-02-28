#include "Background.h"
#include "Util/Util.h"


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
	else {}
		//mainView.move(sf::Vector2f(.8f * gameSpeed, -.4f * gameSpeed));
		mainView.move(translateTo2d(sf::Vector3f(0, 0, -1.3 * gameSpeed)));//for translateTo2d

	window.draw(back);
}


void Background::setPosition(sf::Vector2f pos)
{
	back.setPosition(pos);
}


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

bool Background::backgroundFinished(sf::View& view)
{
	//float wXPos = view.getCenter().x + (view.getSize().x / 2); // temp
	float wXPos = view.getCenter().x - (view.getSize().x / 2);

	return wXPos >= 1830;
	//return wXPos >= back.getGlobalBounds().width; // temp
}


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
		back.setPosition(sf::Vector2f(0, 240));
		player.resetPos(startPos);
	}
}


// static
void Background::generateObstacles(Background::Stage stage,
	std::vector<Obstacle*>& obstacles, sf::Texture* spriteSheet)
{
	/*Shooting Obstacles
	KEY
	0 = Grey Turrets
	1 = Green Turrets
	2 = Shooting Up Bullets
	*/
	

	//Testing
	//obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 135.6f, -700.f), spriteSheet, 1, 0));


	/*
	Stationary Obstacles
	KEY
	1 = gas can
	2 = satellite
	3 = plane
	*/
	obstacles.clear();
	switch (stage)
	{
	case INITIAL:
		//TestingGreyCannon::obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 140.6f, -700.f), spriteSheet, 100, 0));
		 
		//Shooting
		obstacles.push_back(new Obstacle(sf::Vector3f(-210.f, 140.6f, -360.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-45.f, 140.6f, -415.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-120.f, 140.6f, -780.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-210.f, 140.6f, -927.f), spriteSheet, 100, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-150.f, 140.6f, -1100.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-210.f, 140.6f, -1125.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-172.f, 140.6f, -1390.f), spriteSheet, 100, 0));
		obstacles.push_back(new Obstacle(sf::Vector3f(-30.f, 140.6f, -1660.f), spriteSheet, 100, 0));

		//Shooting Up Missiles
		obstacles.push_back(new Obstacle(sf::Vector3f(-60.f, 140.6f, -345.f), spriteSheet, 100, 2));

		//Non-Shooting
		obstacles.push_back(new Obstacle(sf::Vector3f(-200.f, 140.6f, -320.f), spriteSheet, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-110.f, 140.6f, -570.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-42.f, 140.6f, -610.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-190.f, 140.6f, -645.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-200.f, 140.6f, -890.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-42.f, 140.6f, -980.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-80.f, 140.6f, -1080.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-35.f, 140.6f, -1285.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-165.f, 140.6f, -1665.f), spriteSheet, 3));
		obstacles.push_back(new Obstacle(sf::Vector3f(-70.f, 140.6f, -1750.f), spriteSheet, 3));

		obstacles.push_back(new Obstacle(sf::Vector3f(-170.f, 140.6f, -1780.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-120.f, 140.6f, -1880.f), spriteSheet, 1));
		break;
	}
}


// static
void Background::generateWaves(Background::Stage stage,
	std::vector<Enemy*>& enemies, sf::Texture* spriteSheet, int playerZ)
{
	enemies.clear();
	
	switch (stage)
	{
	case SPACE:
		Enemy::spawnWave(enemies, spriteSheet, playerZ, 0);
		break;
	}
}