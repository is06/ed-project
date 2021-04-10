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

    controller = new Controller(static_cast<EventManager*>(irrlichtDevice->getEventReceiver()));
    currentScene = new Map(this, "labo");
}

void Game::mainLoop()
{
    u32 realTime = 0.0f;
    f32 lastCycleTime, loopTime = 0.0f;

    while (irrlichtDevice->run()) {
        realTime = irrlichtDevice->getTimer()->getRealTime();
        lastCycleTime = realTime - loopTime;
        loopTime = realTime;

        videoDriver->beginScene();
        currentScene->update(lastCycleTime / 1000.0f);
        sceneManager->drawAll();
        videoDriver->endScene();

        irrlichtDevice->yield();
    }
}

scene::ISceneManager* Game::getSceneManager()
{
    return sceneManager;
}

Controller* Game::getController()
{
    return controller;
}

Game::~Game()
{
    delete currentScene;
    delete controller;

    irrlichtDevice->drop();
}
