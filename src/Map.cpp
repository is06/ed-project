#include "Game.h"
#include "Map.h"
#include "Scene.h"
#include "entities/Camera.h"
//#include "entities/Model.h"
#include "entities/Player.h"

Map::Map(Game* game) : Scene(game)
{
    player = new Player(this);
    camera = new Camera(this);

    //entities["cube"] = new Model(this, "cube");
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

Map::~Map()
{
    delete player;

    for (auto& pair : entities) {
        delete pair.second;
    }
}