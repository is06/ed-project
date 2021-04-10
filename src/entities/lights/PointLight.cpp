#include "PointLight.h"

PointLight::PointLight(Scene* scene, video::SColor color, f32 radius) : Light(scene)
{
    node = scene->getSceneManager()->addLightSceneNode();
    node->setDebugDataVisible(scene::EDS_FULL);
    node->setPosition(core::vector3df(0, 1.0f, 0));

    auto light = static_cast<scene::ILightSceneNode*>(node);
    light->setRadius(radius);
    light->setLightType(video::ELT_POINT);

    video::SLight data;
    data.DiffuseColor = color;
    data.AmbientColor = color;
    data.Type = video::ELT_POINT;
    data.Radius = radius;
    light->setLightData(data);
}

void PointLight::update(f32 speed)
{

}

PointLight::~PointLight()
{

}
