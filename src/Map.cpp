#include "Game.h"
#include "Map.h"
#include "Scene.h"
#include "entities/Player.h"
#include "entities/cameras/TpCamera.h"
#include "entities/lights/PointLight.h"

Map::Map(Game* game, const string& name) : Scene(game)
{
    this->name = name;    

    world = new World(this);
    player = new Player(this);
    camera = new TpCamera(player, controller);

    gravity = core::vector3df(0, -1.0f, 0);

    //sceneManager->setAmbientLight(video::SColor(255, 128, 128, 128));
    player->attachWorldForCollisions(world);

    addStatic("vc")
        ->position(0, 2, 0);
}

void Map::update(f32 speed)
{
    Scene::update(speed);

    world->update(speed);
    player->update(speed);
    camera->update(speed);

    for (auto& pair : entities) {
        pair.second->update(speed);
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

StaticModel* Map::addStatic(const string& name)
{
    auto model = new StaticModel(this, name);
    entities[name] = model;
    return model;
}

Light* Map::addLight(const string& name)
{
    auto light = new PointLight(this, video::SColor(255, 255, 255, 255), 32.0f);
    entities[name] = light;
    return light;
}

Map::~Map()
{
    delete player;
    delete camera;
    delete world;

    for (auto& pair : entities) {
        delete pair.second;
    }
}