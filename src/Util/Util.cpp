#include "Util.h"


// could be changed to a static method on a class later if want
sf::Vector2f translateTo2d(sf::Vector3f in) 
{
	int x = -in.x*0.5 + -in.z*0.5;
	int y = in.y + -in.x*0.5 + in.z*0.5;

	return sf::Vector2f(x, y);
}