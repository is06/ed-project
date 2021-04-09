#include "MapZone.h"
#include "../GameException.h"

MapZone::MapZone(Map* map, const string& name)
    : Model(map, "content/models/maps/" + map->getName() + "/" + name + ".obj")
{
    node = map->getSceneManager()->addMeshSceneNode(mesh);

    if (node == nullptr) {
        throw GameException("Unable to create map zone node");
    }

    node->setMaterialFlag(video::EMF_LIGHTING, false);
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
}

MapZone::~MapZone()
{
    node->remove();
}
