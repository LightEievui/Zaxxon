#include <SFML/Graphics.hpp>
#include "Game.h"


int main()
{
    Game* game = new Game();
    game->run();
    delete game;
    return 0;
}