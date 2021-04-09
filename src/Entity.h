#ifndef ENTITY_H
#define ENTITY_H

#include <irrlicht/irrlicht.h>

#include "Scene.h"

using namespace irr;

class Scene;

class Entity
{
public:
    Entity(Scene* scene);
    virtual void update();
    virtual void draw();
    virtual ~Entity();

protected:
    scene::ISceneNode* node;
};

#endif