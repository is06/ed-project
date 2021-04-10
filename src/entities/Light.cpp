#include "Light.h"

Light::Light(Scene* scene) : Entity(scene)
{

}

void Light::update(f32 speed)
{
    Entity::update(speed);
}

Light::~Light()
{

}
