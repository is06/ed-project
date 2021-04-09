#ifndef GAME_H
#define GAME_H

#include <irrlicht/irrlicht.h>

using namespace irr;

class Game
{
public:
    Game();
    ~Game();

private:
    IrrlichtDevice* irrlichtDevice;
};

#endif
