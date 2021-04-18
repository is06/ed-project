#include "World.h"
#include "../GameException.h"

World::World(Map* map)
    : Model(map, "content/maps/" + map->getName() + "/mesh.cwf")
{
    node = map->getSceneManager()->addMeshSceneNode(mesh);
    node->setMaterialFlag(video::EMF_LIGHTING, false);

    if (node == nullptr) {
        throw GameException("Unable to create map zone node");
    }
}

void World::update(f32 speed)
{
    Model::update(speed);
}

World::~World()
{
    node->remove();
}
