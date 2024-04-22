#include "Util.h"

/// <summary>
/// Convert normal 2d coordinates to isometric coordinates.
/// </summary>
/// <param name="in"></param>
/// <returns>Vector of 2 floats</returns>
sf::Vector2f translateTo2d(sf::Vector3f in)
{
	float x = -in.x * 0.6f + -in.z * 0.6f;
	float y = in.y + -in.x * 0.3f + in.z * 0.3f;

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

sf::Font debugFont;