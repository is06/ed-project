#include "TpCamera.h"
#include "../../Direction.h"

TpCamera::TpCamera(Player* player, Controller* controller) : Camera(player->getMap())
{
    this->player = player;
    this->controller = controller;

    auto camera = static_cast<scene::ICameraSceneNode*>(node);
    camera->bindTargetAndRotation(true);
    player->attachThirdPersonCamera(this);

    rotation = Direction::up;
    height = MAX_HEIGHT;
    distance = MAX_FAR;
}

void TpCamera::update(f32 speed)
{
    Camera::update(speed);

    auto camera = static_cast<scene::ICameraSceneNode*>(node);
    camera->setTarget(core::vector3df(
        player->getNode()->getPosition().X,
        player->getNode()->getPosition().Y + 1.7f,
        player->getNode()->getPosition().Z
    ));

    f32 xAxis = controller->getCameraXAxis();
    f32 yAxis = controller->getCameraYAxis();

    if (xAxis < -35.0f) {
        goLeft(speed * xAxis * -1);
    } else if (xAxis > 35.0f) {
        goRight(speed * xAxis);
    }

    if (yAxis > 35.0f) {
        goNear(speed * yAxis);
    } else if (yAxis < -35.0f) {
        goFar(speed * yAxis * -1);
    }

    f32 x = player->getNode()->getPosition().X + distance * cos(rotation);
    f32 z = player->getNode()->getPosition().Z + distance * sin(rotation);

    node->setPosition(core::vector3df(x, player->getNode()->getPosition().Y + height, z));
}

void TpCamera::goLeft(f32 speed)
{
    rotation -= (speed / ROTATION_SPEED);
    if (rotation < 0) {
        rotation = 2 * core::PI;
    }
}

void TpCamera::goRight(f32 speed)
{
    rotation += (speed / ROTATION_SPEED);
    if (rotation > (2 * core::PI)) {
        rotation = 0;
    }
}

void TpCamera::goNear(f32 speed)
{
    if (height > MIN_HEIGHT) {
        height -= (speed / DISTANCE_SPEED);
    }
    if (height < MIN_HEIGHT) {
        height = MIN_HEIGHT;
    }
    if (distance > MIN_NEAR) {
        distance -= (speed / DISTANCE_SPEED);
    } else {
        distance = MIN_NEAR;
    }
}

void TpCamera::goFar(f32 speed)
{
    if (height < MAX_HEIGHT) {
        height += (speed / DISTANCE_SPEED);
    }
    if (height > MAX_HEIGHT) {
        height = MAX_HEIGHT;
    }
    if (distance < MAX_FAR) {
        distance += (speed / DISTANCE_SPEED);
    } else {
        distance = MAX_FAR;
    }
}

TpCamera::~TpCamera()
{

}
