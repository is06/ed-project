#ifndef GAME_H
#define GAME_H

#include <irrlicht/irrlicht.h>

using namespace irr;

class Game
{
public:
    static void run();
    ~Game();

private:
    Game();
    void mainLoop();

    IrrlichtDevice* irrlichtDevice;
    video::IVideoDriver* videoDriver;
    scene::ISceneManager* sceneManager;
};

#endif
