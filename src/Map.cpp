#include "Game.h"
#include "Map.h"
#include "Scene.h"

Map::Map(Game* game) : Scene(game)
{
    
}

void Map::update()
{
    Scene::update();
}

void Map::draw()
{
    Scene::draw();
}

Map::~Map()
{

}