#include "Light.h"

Light::Light(Scene* scene) : Entity(scene)
{

}

void Light::update(f32 speed)
{
    Entity::update(speed);
}

Light* Light::setColor(video::SColor color)
{
    auto light = static_cast<scene::ILightSceneNode*>(node);
    light->getLightData().DiffuseColor = color;
    return this;
}

Light* Light::setRadius(f32 radius)
{
    auto light = static_cast<scene::ILightSceneNode*>(node);
    light->setRadius(radius);
    return this;
}

Light* Light::setData(const video::SLight& lightData)
{
    auto light = static_cast<scene::ILightSceneNode*>(node);
    light->setLightData(lightData);
    return this;
}

Light::~Light()
{

}
