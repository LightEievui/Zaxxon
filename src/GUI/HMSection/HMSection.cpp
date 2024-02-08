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
	case TOP:
		setSection(6);
		break;
	case BOTTOM:
		setSection(4);
		break;
	}
}


int HMSection::getSection()
{
	return section;
}

void HMSection::progress(int stage)
{
	int change;

	if (stage >= 0)
	{
		switch (this->type)
		{
		case TOP:
		case LINE:
			change = 16 - stage;
			setSection(change); // full 9-16 to 5-8: blank to full so 8-5 then 16-9: full to blank
			break;
		case OPEN:
			change = 24 - stage;
			setSection(change); // 17-24 to 0-3: blank to full so 3-0 then 24-17: full to blank
			break;
		case BOTTOM:
			break;
		}
	}
	else
	{
		switch (this->type)
		{
		case TOP:
		case LINE:
			change = 8+stage+1;
			setSection(change); // 
			break;
		case OPEN:
			change = 3+stage+1;
			setSection(change); // 
			break;
		case BOTTOM:
			break;
		}
	}
	
}


void HMSection::fill()
{
	switch (this->type)
	{
	case LINE:
		setSection(8);
		break;
	case OPEN:
		setSection(3);
		break;
	case TOP:
		setSection(5);
	case BOTTOM:
		setSection(4);
		break;
	}
}


void HMSection::empty()
{
	switch (this->type)
	{
	case TOP:
	case LINE:
		setSection(9);
		break;
	case OPEN:
		setSection(17);
		break;
	case BOTTOM:
		setSection(4);
		break;
	}
}