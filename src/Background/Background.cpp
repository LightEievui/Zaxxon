#include "Background.h"
#include "Util/Util.h"


Background::Background(sf::Vector2f pos)
{
	if (!initial.loadFromFile("res/BackgroundInitial.png"))
		std::cout << "Background file could not load\n";
	if (!space.loadFromFile("res/BackgroundSpace2.png"))
		std::cout << "Background file could not load\n";
	if (!boss.loadFromFile("res/BackgroundBoss.png"))
		std::cout << "Background file could not load\n";

	back.setTexture(initial);
	sf::View a = sf::View();
	resetPos(a);
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
			resetPos(mainView);
		}
		else if (stage == Stage::SPACE)
		{
			stage = Stage::BOSS;
			back.setTexture(boss);
			resetPos(mainView);
		}
		else
		{
			stage = Stage::INITIAL;
			back.setTexture(initial);
			resetPos(mainView);
		}
		generateObstacles(stage, obstacles, spritesheet);
		generateWaves(stage, enemies, spritesheet, player.getPos().z);
	}
	else
		//mainView.move(sf::Vector2f(.8f * gameSpeed, -.4f * gameSpeed));
		mainView.move(translateTo2d(sf::Vector3f(0, 0, -1.3 * gameSpeed)));//for translateTo2d

	window.draw(back);
}


void Background::setPosition(sf::Vector2f pos)
{
	back.setPosition(pos);
}


bool Background::backgroundFinished(sf::View& view)
{
	//float wXPos = view.getCenter().x + (view.getSize().x / 2); // temp
	float wXPos = view.getCenter().x - (view.getSize().x / 2);

	return wXPos >= 1830;
	//return wXPos >= back.getGlobalBounds().width; // temp
}


void Background::resetPos(sf::View& mainView)
{
	//Sets the origin to the bottom left corner as that is where it will start 
	//on the screen
	//mainView.setCenter(sf::Vector2f(0, 0));
	mainView.reset(sf::FloatRect(0.f, 0.f, 224.f, 224.f));
	back.setOrigin(sf::Vector2f(0, back.getTexture()->getSize().y));
	back.setPosition(sf::Vector2f(0, 240));
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
	//obstacles.push_back(new Obstacle(sf::Vector3f(-150.f, 135.6f, -470.f), spriteSheet, 1, 1));

	//Testing
	//obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 135.6f, -700.f), spriteSheet, 1, 0));


	/*
	Stationary Obstacles
	KEY
	1 = gas can
	2 = satellite
	*/
	obstacles.clear();
	switch (stage)
	{
	case INITIAL:
		obstacles.push_back(new Obstacle(sf::Vector3f(-100.f, 140.6f, -700.f), spriteSheet, 100, 0));

		obstacles.push_back(new Obstacle(sf::Vector3f(-157.f, 140.6f, -425.f), spriteSheet, 2));
		obstacles.push_back(new Obstacle(sf::Vector3f(-83.f, 140.6f, -625.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-30.f, 140.6f, -630.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-150.f, 140.6f, -745.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-150.f, 140.6f, -995.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-30.f, 140.6f, -990.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-65.f, 140.6f, -1115.f), spriteSheet, 1));
		obstacles.push_back(new Obstacle(sf::Vector3f(-30.f, 140.6f, -1290.f), spriteSheet, 1));
		break;
	}

	
}


// static
void Background::generateWaves(Background::Stage stage,
	std::vector<Enemy*>& enemies, sf::Texture* spriteSheetint, int playerZ)
{
	enemies.clear();
}