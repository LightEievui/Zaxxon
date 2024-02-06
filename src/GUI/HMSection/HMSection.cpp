#include "HMSection.h"


// 25 sections 8 - 207 x
void HMSection::setSection(int sect)
{
	section = sect % 25;
	this->setTextureRect(sf::IntRect(8 + section * 8, 296, 8, 8));
}

void HMSection::setType(Type type)
{
	this->type = type;
	switch(this->type)
	{
	case LINE:
		setSection(8);
		break;
	case OPEN:
		setSection(3);
		break;
	}
}


int HMSection::getSection()
{
	return section;
}
