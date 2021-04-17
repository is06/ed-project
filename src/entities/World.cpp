#include "World.h"
#include "../GameException.h"

World::World(Map* map, const string& name)
    : Model(map, "content/models/maps/" + map->getName() + "/" + name + ".obj")
{
    node = map->getSceneManager()->addMeshSceneNode(mesh);

    if (node == nullptr) {
        throw GameException("Unable to create map zone node");
    }

    node->getMaterial(0).Shininess = 0;
    node->getMaterial(0).SpecularColor = video::SColor(0, 0, 0, 0);
}

World::~World()
{
    node->remove();
}