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

    Light* setColor(video::SColor);
    Light* setRadius(f32 radius);
    Light* setData(const video::SLight& lightData);
};

#endif
