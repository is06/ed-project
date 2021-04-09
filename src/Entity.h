#ifndef ENTITY_H
#define ENTITY_H

#include "Scene.h"

class Scene;

class Entity
{
public:
    Entity(Scene* scene);
    virtual void update();
    virtual void draw();
    virtual ~Entity();
};

#endif