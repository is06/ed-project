#ifndef MAP_H
#define MAP_H

#include <string>
#include <unordered_map>

#include "Entity.h"
#include "Game.h"
#include "Scene.h"
#include "entities/Camera.h"
#include "entities/MapZone.h"
#include "entities/Player.h"

using namespace std;

class Camera;
class Entity;
class Game;
class MapZone;
class Player;
class Scene;

class Map : public Scene
{
public:
    Map(Game* game, const string& name);
    virtual void update();
    virtual void draw();
    virtual ~Map();

    const string& getName() const;

private:
    string name;
    Player* player;
    Camera* camera;
    unordered_map<string, MapZone*> zones;
    unordered_map<string, Entity*> entities;
};

#endif
