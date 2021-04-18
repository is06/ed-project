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
    virtual void update(f32 speed);
    virtual void draw();
    virtual ~Entity();

    scene::ISceneNode* getNode();
    Entity* setPosition(f32 x, f32 y, f32 z);
    virtual void setDebugFlags(bool active);

protected:
    Scene* scene;
    scene::ISceneNode* node;
};

#endif