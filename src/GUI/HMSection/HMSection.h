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

	// TODO
	// void progress();
	void setSection(int sect);
	void setType(Type type);
	int getSection();
private:
	int section;
	int type;
};

