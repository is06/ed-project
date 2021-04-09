#ifndef MAP_H
#define MAP_H

#include <string>
#include <unordered_map>

#include "Entity.h"
#include "Game.h"
#include "Scene.h"
#include "entities/Camera.h"
#include "entities/Player.h"

using namespace std;

class Camera;
class Entity;
class Game;
class Player;
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
    Player* player;
    Camera* camera;
};

#endif
