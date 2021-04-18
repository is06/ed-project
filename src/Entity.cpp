#include "Entity.h"
#include "Scene.h"

Entity::Entity(Scene* scene)
{
    this->scene = scene;
}

void Entity::update(f32 speed)
{

}

void Entity::draw()
{

}

Entity* Entity::setPosition(f32 x, f32 y, f32 z)
{
    node->setPosition(core::vector3df(x, y, z));

    return this;
}

scene::ISceneNode* Entity::getNode()
{
    return node;
}

void Entity::setDebugFlags(bool active)
{
    node->setMaterialFlag(video::EMF_WIREFRAME, active);
}

Entity::~Entity()
{

}
