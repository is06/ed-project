#include "TpCamera.h"

TpCamera::TpCamera(Player* player) : Camera(player->getScene())
{
    this->player = player;

    auto camera = static_cast<scene::ICameraSceneNode*>(node);
    camera->bindTargetAndRotation(true);
    camera->setTarget(player->getNode()->getPosition());

    rotation = (3 * core::PI) / 2;
    height = 1.8f;
    distance = 4.0f;
}

void TpCamera::update()
{
    Camera::update();



    f32 x = player->getNode()->getPosition().X + distance * cos(rotation);
    f32 z = player->getNode()->getPosition().Y + distance * sin(rotation);

    node->setPosition(core::vector3df(x, player->getNode()->getPosition().Y + height, z));
}

void TpCamera::goLeft(f32 speed)
{
    rotation -= (speed / 60.0f);
    if (rotation < 0) {
        rotation = 2 * core::PI;
    }
}

void TpCamera::goRight(f32 speed)
{
    rotation += (speed / 60.0f);
    if (rotation > (2 * core::PI)) {
        rotation = 0;
    }
}

void TpCamera::goNear(f32 speed)
{
    if (height > 0.3f) {
        height -= (speed / 50.0f);
    }
    if (height < 0.3f) {
        height = 0.3f;
    }
    if (distance > 2.5f) {
        distance -= (speed / 50.0f);
    } else {
        distance = 2.5f;
    }
}

void TpCamera::goFar(f32 speed)
{
    if (height < 3.3f) {
        height += (speed / 50.0f);
    }
    if (height > 3.3f) {
        height = 3.3f;
    }
    if (distance < 5.5f) {
        distance += (speed / 50.0f);
    } else {
        distance = 5.5f;
    }
}

TpCamera::~TpCamera()
{

}
