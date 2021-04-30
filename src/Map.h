#ifndef MAP_H
#define MAP_H

#include <string>
#include <unordered_map>
#include <irrlicht/irrlicht.h>

#include "Entity.h"
#include "Game.h"
#include "Scene.h"
#include "entities/Camera.h"
#include "entities/Light.h"
#include "entities/Player.h"
#include "entities/World.h"
#include "entities/objects/StaticModel.h"

using namespace irr;
using namespace std;

class Camera;
class Entity;
class Game;
class Light;
class Player;
class Scene;
class StaticModel;
class World;

class Map : public Scene
{
public:
    Map(Game* game, const string& name);
    virtual void update(f32 speed);
    virtual void draw();
    virtual ~Map();

    const string& getName() const;
    void setDebugFlags(bool active);

    core::vector3df gravity;

private:
    StaticModel* addStatic(const string& name);
    Light* addLight(const string& name);
    Light* getLight(const string& name);

    string name;
    Player* player;
    Camera* camera;
    World* world;
    unordered_map<string, Entity*> entities;
};

#endif
