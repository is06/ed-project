#ifndef LIGHT_H
#define LIGHT_H

#include "../Entity.h"
#include "../Scene.h"

class Entity;
class Scene;

class Light : public Entity
{
public:
    Light(Scene* scene);
    virtual void update(f32 speed);
    virtual ~Light();

    Light* position(f32 x, f32 y, f32 z);
    Light* color(video::SColor);
    Light* radius(f32 radius);
    Light* data(const video::SLight& lightData);
};

#endif
