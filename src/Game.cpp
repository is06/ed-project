#include <irrlicht/irrlicht.h>
#include "Game.h"

using namespace irr;

void Game::run()
{
    Game g = Game();
    g.mainLoop();
}

Game::Game()
{
    SIrrlichtCreationParameters params;
    params.Bits = 32;
    params.WindowSize = core::dimension2du(1280, 720);

    irrlichtDevice = createDeviceEx(params);
    irrlichtDevice->setWindowCaption(L"Ed project (Tonic Trouble like game)");
    videoDriver = irrlichtDevice->getVideoDriver();
    sceneManager = irrlichtDevice->getSceneManager();
}

void Game::mainLoop()
{
    while (irrlichtDevice->run()) {
        videoDriver->beginScene();
        sceneManager->drawAll();
        videoDriver->endScene();

        irrlichtDevice->yield();
    }
}

Game::~Game()
{
    irrlichtDevice->drop();
}
