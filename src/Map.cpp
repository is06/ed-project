#include "Game.h"
#include "Map.h"
#include "Scene.h"
#include "entities/Player.h"
#include "entities/cameras/TpCamera.h"
#include "entities/lights/PointLight.h"

Map::Map(Game* game, const string& name) : Scene(game)
{
    this->name = name;

    sceneManager->setAmbientLight(video::SColor(255, 255, 255, 255));

    player = new Player(this);
    camera = new TpCamera(player, controller);

    zones["001"] = new MapZone(this, "zone001");
    //entities["test_light"] = new PointLight(this, video::SColor(255, 255, 255, 0), 0.01f);

    gravity = core::vector3df(0, -10.0f, 0);

    scene::ITriangleSelector* selector = sceneManager->createTriangleSelector(
        zones["001"]->getMesh(),
        zones["001"]->getNode()
    );
    zones["001"]->getNode()->setTriangleSelector(selector);

    if (selector != nullptr) {
        scene::ISceneNodeAnimator* collisionAnimator = sceneManager->createCollisionResponseAnimator(
            selector,
            player->getNode(),
            core::vector3df(1, 1.2f, 1), // radius
            gravity,
            core::vector3df(0, -0.6f, 0) // translation
        );
        selector->drop();
        player->getNode()->addAnimator(collisionAnimator);
        collisionAnimator->drop();
    }
}

void Map::update(f32 speed)
{
    Scene::update(speed);

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