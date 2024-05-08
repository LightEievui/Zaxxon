/*
* Time of development:
* Jan 29 - Apr 30 2024
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

// memory detection (temp)
#include <vld.h>

// temp
#define _CRTDBG_MAP_ALLOC

#include <SFML/Graphics.hpp>
#include "Game/Game.h"
#include <iostream>
#include <crtdbg.h>
#ifdef _WIN32
#include <windows.h>
#endif

// temp
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

int main()
{
#if defined _WIN32 && defined NDEBUG
	FreeConsole();
#endif
	// Startup Zaxxon
	auto game = new Game();
	game->run();

	// Game finished, remove it from memory
	delete game;

	// temp
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	std::cout << _CrtDumpMemoryLeaks();
	return 0;
}
