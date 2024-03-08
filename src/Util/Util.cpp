#include "Util.h"
// TODO: Find f(0) : sf::Vector3f(in.x, ~, in.z) such that x = cos 0 and y = sin 0 

/// <summary>
/// Convert normal 2d coordinates to isometric coordinates.
/// </summary>
/// <param name="in"></param>
/// <returns>Vector of 2 floats</returns>
sf::Vector2f translateTo2d(sf::Vector3f in) 
{
	// could be changed to a static method on a class later if want
	double x = -in.x * 0.6f + -in.z * 0.6f;
	double y = in.y + -in.x * 0.3f + in.z * 0.3f;

	return sf::Vector2f(x, y);
}


/// <summary>
/// Convert normal 2d coordinates to isometric coordinates.
/// This one is specifically for the player character.
/// </summary>
/// <param name="in"></param>
/// <returns>Vector of 2 floats</returns>
sf::Vector2f translateTo2d2(sf::Vector3f in)
{
	// For player
	float x = -in.x * 0.6f + -in.z * 0.4f;
	float y = in.y + -in.x * 0.4f + in.z * 0.2f;

	return sf::Vector2f(x, y);
}


/// <summary>
/// Easily get the current view rectangle with translations.
/// </summary>
/// <param name="window"></param>
/// <returns>Float rectangle</returns>
sf::FloatRect getWindowViewRect(sf::RenderWindow& window)
{
	sf::Vector2f wPos = sf::Vector2f(window.getView().getCenter().x - (window.getView().getSize().x / 2),
		window.getView().getCenter().y - (window.getView().getSize().y / 2));
	return sf::FloatRect(wPos.x, wPos.y, window.getView().getSize().x,
		window.getView().getSize().y);
}