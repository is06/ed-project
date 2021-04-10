#include "Entity.h"
#include "Scene.h"

Entity::Entity(Scene* scene)
{
    this->scene = scene;
}

void Entity::update()
{

}

void Entity::draw()
{

}

scene::ISceneNode* Entity::getNode()
{
    return node;
}

Entity::~Entity()
{

}
