#ifndef GAME_H
#define GAME_H

#include <irrlicht/irrlicht.h>
#include "Scene.h"

using namespace irr;

class Scene;

class Game
{
public:
    static void run();
    ~Game();

    scene::ISceneManager* getSceneManager();

private:
    Game();
    void mainLoop();

    IrrlichtDevice* irrlichtDevice;
    video::IVideoDriver* videoDriver;
    scene::ISceneManager* sceneManager;

    Scene* currentScene;
};

#endif
