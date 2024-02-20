#include "Util.h"


// could be changed to a static method on a class later if want
sf::Vector2f translateTo2d(sf::Vector3f in) 
{
	float x = -in.x*0.3f + -in.z*0.6f;
	float y = in.y + -in.x*0.6f + in.z*0.3f;

	return sf::Vector2f(x, y);
}


// For player
sf::Vector2f translateTo2d2(sf::Vector3f in)
{
	float x = -in.x * 0.6f + -in.z * 0.4f;
	float y = in.y + -in.x * 0.4f + in.z * 0.2f;

	return sf::Vector2f(x, y);
}


sf::FloatRect getWindowViewRect(sf::RenderWindow& window)
{
	sf::Vector2f wPos = sf::Vector2f(window.getView().getCenter().x - (window.getView().getSize().x / 2),
		window.getView().getCenter().y - (window.getView().getSize().y / 2));
	return sf::FloatRect(wPos.x, wPos.y, window.getView().getSize().x,
		window.getView().getSize().y);
}