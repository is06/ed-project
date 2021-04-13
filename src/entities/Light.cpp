#include "Light.h"

Light::Light(Scene* scene) : Entity(scene)
{

}

void Light::update(f32 speed)
{
    Entity::update(speed);
}

Light* Light::position(f32 x, f32 y, f32 z)
{
    node->setPosition(core::vector3df(x, y, z));
    return this;
}

Light* Light::color(video::SColor color)
{
    auto light = static_cast<scene::ILightSceneNode*>(node);
    light->getLightData().DiffuseColor = color;
    return this;
}

Light* Light::radius(f32 radius)
{
    auto light = static_cast<scene::ILightSceneNode*>(node);
    light->setRadius(radius);
    return this;
}

Light* Light::data(const video::SLight& lightData)
{
    auto light = static_cast<scene::ILightSceneNode*>(node);
    light->setLightData(lightData);
    return this;
}

Light::~Light()
{

}
