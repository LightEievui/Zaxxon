#include <SFML/Graphics.hpp>
#pragma once


/// <summary>
/// The height meter and related methods.
/// </summary>
class HMSection : public sf::Sprite
{
public:
	enum Type
	{
		LINE,
		OPEN,
		TOP,
		BOTTOM
	};

	void progress(int);
	void setSection(int);
	void setType(Type);
	void fill();
	void empty();

	int getSection();
private:
	int section = 0;
	int type = 0;
};