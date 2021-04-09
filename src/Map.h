#ifndef MAP_H
#define MAP_H

#include <string>
#include <unordered_map>

#include "Entity.h"
#include "Game.h"
#include "Scene.h"

using namespace std;

class Entity;
class Game;
class Scene;

class Map : public Scene
{
public:
    Map(Game* game);
    virtual void update();
    virtual void draw();
    virtual ~Map();

private:
    unordered_map<string, Entity*> entities;
};

#endif
