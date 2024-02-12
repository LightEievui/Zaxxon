#include "Util.h"


// could be changed to a static method on a class later if want
sf::Vector2f translateTo2d(sf::Vector3f in) 
{
	float x = -in.x*0.3f + -in.z*0.6f;
	float y = in.y + -in.x*0.6f + in.z*0.3f;

	return sf::Vector2f(x, y);
}


sf::Vector2f translateTo2d2(sf::Vector3f in)
{
	float x = -in.x * 0.6f + -in.z * 0.4f;
	float y = in.y + -in.x * 0.4f + in.z * 0.6f;

	return sf::Vector2f(x, y);
}