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


/// <summary>
/// Detects if main button is pressed, automatically gets rid of
/// inputs that are a held key over first press.
/// </summary>
/// <returns>A boolean</returns>
bool zPressed()
{
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Z) ||
		sf::Joystick::isButtonPressed(0, 0)) && util_zPress)
	{
		util_zPress = false;
		return true;
	}

	else if (!util_zPress && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Joystick::isButtonPressed(0, 0)))
		util_zPress = true;

	return false;
}


/// <summary>
/// Detects if up is current pressed, will handle both keyboard and controller.
/// This will consider holding the stick in this direction as constant 'presses'
/// </summary>
/// <returns>A boolean</returns>
bool upPressed()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
		sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -10;
}


/// <summary>
/// Detects if down is current pressed, will handle both keyboard and controller.
/// This will consider holding the stick in this direction as constant 'presses'
/// </summary>
/// <returns>A boolean</returns>
bool downPressed()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
		sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 10;
}


/// <summary>
/// Detects if left is current pressed, will handle both keyboard and controller.
/// This will consider holding the stick in this direction as constant 'presses'
/// </summary>
/// <returns>A boolean</returns>
bool leftPressed()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -10;
}


/// <summary>
/// Detects if right is current pressed, will handle both keyboard and controller.
/// This will consider holding the stick in this direction as constant 'presses'
/// </summary>
/// <returns>A boolean</returns>
bool rightPressed()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
		sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 10;
}


sf::Font debugFont;