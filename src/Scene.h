#ifndef SCENE_H
#define SCENE_H

#include <irrlicht/irrlicht.h>

#include "Game.h"

using namespace irr;

class Game;

class Scene
{
public:
    Scene(Game* game);
    virtual void update();
    virtual void draw();
    virtual ~Scene();

    scene::ISceneManager* getSceneManager();

protected:
    Game* game;
    scene::ISceneManager* sceneManager;
};

#endif
