#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "../Light.h"

class PointLight : public Light
{
public:
    PointLight(Scene* scene, video::SColor color, f32 radius);
    virtual void update(f32 speed);
    virtual ~PointLight();
};

#endif
