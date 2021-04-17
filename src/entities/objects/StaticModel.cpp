#include "StaticModel.h"

StaticModel::StaticModel(Map* map, const string& name)
    : Model(map, "content/models/" + name + ".cwf")
{
    node = map->getSceneManager()->addMeshSceneNode(mesh);
}

void StaticModel::update(f32 speed)
{
    Model::update(speed);
}

StaticModel::~StaticModel()
{
    node->remove();
}
