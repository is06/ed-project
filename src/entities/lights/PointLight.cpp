#include "PointLight.h"

PointLight::PointLight(Scene* scene, video::SColor color, f32 radius) : Light(scene)
{
    node = scene->getSceneManager()->addLightSceneNode();
    node->setDebugDataVisible(scene::EDS_FULL);
    node->setPosition(core::vector3df(0, 1.0f, 0));

    video::SLight data;
    data.Type = video::ELT_POINT;
    data.DiffuseColor = color;
    data.AmbientColor = color;
    data.Radius = radius;

    auto light = static_cast<scene::ILightSceneNode*>(node);
    light->setLightData(data);
}

void PointLight::update(f32 speed)
{

}

PointLight::~PointLight()
{
    node->remove();
}
