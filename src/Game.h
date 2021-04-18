#ifndef GAME_H
#define GAME_H

#include <irrlicht/irrlicht.h>

#include "Controller.h"
#include "Scene.h"

using namespace irr;

class Controller;
class Scene;

class Game
{
public:
    static void run();
    ~Game();

    scene::ISceneManager* getSceneManager();
    Controller* getController();

    bool isInDebugMode;

private:
    Game();
    void mainLoop();

    IrrlichtDevice* irrlichtDevice;
    video::IVideoDriver* videoDriver;
    scene::ISceneManager* sceneManager;

    Controller* controller;
    Scene* currentScene;
};

#endif
