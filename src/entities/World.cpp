#include "World.h"
#include "../GameException.h"

World::World(Map* map)
    : Model(map, "content/maps/" + map->getName() + "/mesh.cwf")
{
    node = map->getSceneManager()->addMeshSceneNode(mesh);

    if (node == nullptr) {
        throw GameException("Unable to create map zone node");
    }

    node->getMaterial(0).Shininess = 0;
    node->getMaterial(0).SpecularColor = video::SColor(0, 0, 0, 0);
}

void World::update(f32 speed)
{
    Model::update(speed);
}

World::~World()
{
    node->remove();
}
