#include "Util.h"


// could be changed to a static method on a class later if want
sf::Vector2f translateTo2d(sf::Vector3f in) 
{
	int x = -in.x*0.3 + -in.z*0.6;
	int y = in.y + -in.x*0.6 + in.z*0.3;

	return sf::Vector2f(x, y);
}