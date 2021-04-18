#include <irrlicht/irrlicht.h>
#include "EventManager.h"
#include "Game.h"
#include "Map.h"
#include "loaders/ColoredWaveFrontLoader.h"

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
    params.Vsync = true;
    params.DriverType = video::EDT_OPENGL;
    params.EventReceiver = new EventManager();

    irrlichtDevice = createDeviceEx(params);
    videoDriver = irrlichtDevice->getVideoDriver();
    sceneManager = irrlichtDevice->getSceneManager();
    
    // Modified WaveFront (OBJ) format loader (with vertex colors data)
    auto coloredWaveFrontLoader = new ColoredWaveFrontLoader(sceneManager, irrlichtDevice->getFileSystem());
    sceneManager->addExternalMeshLoader(coloredWaveFrontLoader);

    controller = new Controller(static_cast<EventManager*>(irrlichtDevice->getEventReceiver()));
    currentScene = new Map(this, "labo");
}

void Game::mainLoop()
{
    u32 lastCycleTime, lastFPS, loopTime, realTime = 0.0f;

    while (irrlichtDevice->run()) {
        // Time computation
        realTime = irrlichtDevice->getTimer()->getRealTime();
        lastCycleTime = realTime - loopTime;
        loopTime = realTime;

        // Display FPS
        lastFPS = videoDriver->getFPS();
        core::stringw title = L"Ed project (Tonic Trouble like game) - ";
        title += lastFPS;
        irrlichtDevice->setWindowCaption(title.c_str());

        // Global controls
        //controller->isActionPerformed(ACTION)

        // Scene rendering
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
