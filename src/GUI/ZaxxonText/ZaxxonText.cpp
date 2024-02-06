#include "ZaxxonText.h"


sf::Sprite ZaxxonText::get(sf::Texture* spritemap, char in)
{
	// chars: 65-90 inclusive
	// nums: 48-57 inclusive
	in = toupper(in);
	if ((in < 65 || in > 90) && (in < 48 || in > 57))
		throw std::invalid_argument("Get char must be in the correct range! Check ZaxxonText.");
	int adjustment;
	if ((in >= 65 && in <= 90))
		adjustment = in - 'A';
	else
		adjustment = in - '0';

	sf::Sprite retVal;
	
	retVal.setTexture(*spritemap);
	retVal.setTextureRect(sf::IntRect(8+8*adjustment, 232, 8, 8));

	return retVal;
}