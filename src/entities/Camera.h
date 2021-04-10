#ifndef CAMERA_H
#define CAMERA_H

#include "../Entity.h"
#include "../Scene.h"

class Camera : public Entity
{
public:
    Camera(Scene* scene);
    virtual void update(f32 speed);
    virtual ~Camera();
};

#endif
