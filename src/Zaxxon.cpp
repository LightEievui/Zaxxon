#include <SFML/Graphics.hpp>
#include "Game/Game.h"


int main()
{
    // Startup Zaxxon
    Game* game = new Game();
    game->run();

    // Game finished, remove it from memory
    delete game;
    return 0;
}