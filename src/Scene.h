#ifndef SCENE_H
#define SCENE_H

#include <irrlicht/irrlicht.h>

#include "Controller.h"
#include "Game.h"

using namespace irr;

class Controller;
class Game;

class Scene
{
public:
    Scene(Game* game);
    virtual void update(f32 speed);
    virtual void draw();
    virtual ~Scene();

    scene::ISceneManager* getSceneManager();
    Controller* getController();

protected:
    Game* game;
    Controller* controller;
    scene::ISceneManager* sceneManager;
};

#endif
