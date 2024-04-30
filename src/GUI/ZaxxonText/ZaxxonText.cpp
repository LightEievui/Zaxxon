#include "ZaxxonText.h"


/// <summary>
/// Take character or hex value then convert it and return
/// the matching character/symbol from our spritesheet.
/// </summary>
/// <param name="spritemap"></param>
/// <param name="in"></param>
/// <returns>Sprite of character or symbol</returns>
sf::Sprite ZaxxonText::get(sf::Texture* spritemap, char in)
{
	// chars: 65-90 inclusive
	// nums: 48-57 inclusive
	// other: 0-31 inclusive
	in = toupper(in);
	//if ((in < 65 || in > 90) && (in < 48 || in > 57) && (in < 0 || in > 31))
	//	throw std::invalid_argument("Get char must be in the correct range! Check ZaxxonText.");
	int column = 0, row = 0;
	if (in >= 65 && in <= 90)
		column = in - 'A';
	else if (in >= 48 && in <= 57)
		column = in - '0' + 26;
	else
		column = in - 1, row = 16;

	sf::Sprite retVal;

	retVal.setTexture(*spritemap);
	retVal.setTextureRect(sf::IntRect(8 + 8 * column, 232 + row, 8, 8));

	return retVal;
}


/// <summary>
/// Take a string of characters or hex values then convert all and then fill
/// given sprite array with the matching characters/symbols for each.
/// </summary>
/// <param name="spritemap"></param>
/// <param name="in"></param>
/// <param name="arr"></param>
void ZaxxonText::string(sf::Texture* spritemap, std::string in, sf::Sprite* arr)
{
	for (unsigned int i = 0; i < in.size(); i++)
		arr[i] = get(spritemap, in[i]);
}