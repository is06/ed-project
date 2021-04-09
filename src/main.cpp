#include <stdio.h>
#include "Game.h"

#include <exception>

using namespace std;

int main()
{
    try {
        Game::run();
    } catch (exception& e) {
        printf(e.what());
    }

    return 0;
}
