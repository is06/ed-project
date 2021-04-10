#include "Camera.h"
#include "../Scene.h"

Camera::Camera(Scene* scene) : Entity(scene)
{
    node = scene->getSceneManager()->addCameraSceneNode();
    node->setPosition(core::vector3df(8, 8, -6));
}

void Camera::update()
{
    Entity::update();
}

Camera::~Camera()
{
    node->remove();
}
