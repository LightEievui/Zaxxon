#include <SFML/Graphics.hpp>
#pragma once


// height meter section
class HMSection : public sf::Sprite
{
public:
	enum Type {
		LINE,
		OPEN,
		TOP,
		BOTTOM
	};

	/*
		Changes the selection by a certain amount. Higher stage: less full.
		Stage must be between 0-7.
	*/
	void progress(int stage);
	void setSection(int sect);
	void setType(Type type);
	void fill();
	void empty();

	int getSection();
private:
	int section;
	int type;
};

