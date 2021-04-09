#include "Camera.h"
#include "../Scene.h"

Camera::Camera(Scene* scene) : Entity(scene)
{
    node = scene->getSceneManager()->addCameraSceneNode();
    node->setPosition(core::vector3df(5, 5, -10));

    auto camera = static_cast<scene::ICameraSceneNode*>(node);
    camera->bindTargetAndRotation(true);
    camera->setTarget(core::vector3df(0, 0, 0));
}

void Camera::update()
{
    Entity::update();
}

Camera::~Camera()
{
    node->remove();
}
