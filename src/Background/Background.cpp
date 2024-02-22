#include "Background.h"


Background::Background(sf::Vector2f pos)
{
	if (!initial.loadFromFile("res/BackgroundInitial.png"))
		std::cout << "Background file could not load\n";
	if (!space.loadFromFile("res/BackgroundSpace.png"))
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


void Background::update(sf::RenderWindow& window, sf::View& mainView, float gameSpeed)
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
	}

	if (!backgroundFinished(mainView))
		mainView.move(sf::Vector2f(.8f * gameSpeed, -.4f * gameSpeed));

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
	mainView.setCenter(sf::Vector2f(112, 112));
	back.setOrigin(sf::Vector2f(0, back.getTexture()->getSize().y));
	back.setPosition(sf::Vector2f(0, 224));
}