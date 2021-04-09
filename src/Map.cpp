#include "Game.h"
#include "Map.h"
#include "Scene.h"
#include "entities/Camera.h"
#include "entities/Player.h"

Map::Map(Game* game, const string& name) : Scene(game)
{
    this->name = name;

    player = new Player(this);
    camera = new Camera(this);

    zones["001"] = new MapZone(this, "zone001");
    //zones["002"] = new MapZone(this, "plane");
}

void Map::update()
{
    Scene::update();

    player->update();

    for (auto& pair : entities) {
        pair.second->update();
    }
}

void Map::draw()
{
    Scene::draw();
}

const string& Map::getName() const
{
    return name;
}

Map::~Map()
{
    delete player;
    delete camera;

    for (auto& pair : zones) {
        delete pair.second;
    }
    for (auto& pair : entities) {
        delete pair.second;
    }
}