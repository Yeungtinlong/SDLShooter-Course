#include "Game.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Game::getInstance().init();
    Game::getInstance().run();

    return 0;
}