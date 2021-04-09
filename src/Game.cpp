#include <irrlicht/irrlicht.h>
#include "Game.h"

using namespace irr;

Game::Game()
{
    SIrrlichtCreationParameters params;

    irrlichtDevice = createDeviceEx(params);
}

Game::~Game()
{
    irrlichtDevice->drop();
}
