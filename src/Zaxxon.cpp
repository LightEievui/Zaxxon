/*
* Time of development:
* Jan 29 - May 8 2024
*
* Developer team:
* Gavin - Team leader / Programmer
* Alex - Administrator / Language lawyer
* Andrew - Chief programmer
* John - Copilot / Tester
*
* Program:
* Recreation of the popular Zaxxon Sega arcade game in C++.
* An easy way to enjoy the classic game with optimized performance
* and memory usage, with all the features as you'd expect.
* 
* Controls:
* Keyboard: Arrow keys and Z
* Joystick: Stick0 and button0
*
* We hereby certify that this program represents our
* work and that the design and logic was completed
* without outside assistance.
*/

#include <SFML/Graphics.hpp>
#include "Game/Game.h"
#include <iostream>


int main()
{
	// Startup Zaxxon
	auto game = new Game();
	game->run();

	// Game finished, remove it from memory
	delete game;
}