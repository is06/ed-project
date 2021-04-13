#include "Game.h"
#include "Map.h"
#include "Scene.h"
#include "entities/Player.h"
#include "entities/cameras/TpCamera.h"
#include "entities/lights/PointLight.h"

Map::Map(Game* game, const string& name) : Scene(game)
{
    this->name = name;

    world = new World(this, "zone001");
    player = new Player(this);
    camera = new TpCamera(player, controller);
    
    addLight("test_light")
        ->position(0, 0, 0)
        ->color(video::SColor(255, 255, 255, 255))
        ->radius(32.0f);

    gravity = core::vector3df(0, -5.0f, 0);
    
    sceneManager->setAmbientLight(video::SColor(255, 255, 255, 255));
}

void Map::initCollisions()
{
    scene::ITriangleSelector* selector = sceneManager->createTriangleSelector(
        world->getMesh(),
        world->getNode()
    );
    world->getNode()->setTriangleSelector(selector);

    if (selector != nullptr) {
        scene::ISceneNodeAnimatorCollisionResponse* collisionAnimator = sceneManager->createCollisionResponseAnimator(
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

void Map::addLight(const string& name)
{
    entities[name] = new PointLight(this, video::SColor(255, 255, 255, 255), 32.0f);
}

void Map::addSpeaker(const string& name)
{

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