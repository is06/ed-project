#include <irrlicht/irrlicht.h>
#include "EventManager.h"
#include "Game.h"
#include "Map.h"

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
    params.EventReceiver = new EventManager();

    irrlichtDevice = createDeviceEx(params);
    irrlichtDevice->setWindowCaption(L"Ed project (Tonic Trouble like game)");
    videoDriver = irrlichtDevice->getVideoDriver();
    sceneManager = irrlichtDevice->getSceneManager();

    currentScene = new Map(this, "labo");
}

void Game::mainLoop()
{
    EventManager* eventManager = static_cast<EventManager*>(irrlichtDevice->getEventReceiver());

    while (irrlichtDevice->run()) {
        if (eventManager->isKeyDown(KEY_ESCAPE)) {
            irrlichtDevice->closeDevice();
        }

        videoDriver->beginScene();
        sceneManager->drawAll();
        videoDriver->endScene();

        irrlichtDevice->yield();
    }
}

scene::ISceneManager* Game::getSceneManager()
{
    return sceneManager;
}

Game::~Game()
{
    delete currentScene;

    irrlichtDevice->drop();
}
