#include "PlatformPlayerAnimator.h"
#include "../Direction.h"

PlatformPlayerAnimator::PlatformPlayerAnimator(Player* player)
{
    this->player = player;
}

void PlatformPlayerAnimator::animateNode(scene::ISceneNode* node, u32 timeMs)
{
    
}

void PlatformPlayerAnimator::jump()
{

}

void PlatformPlayerAnimator::goLeft(f32 speed)
{
    updateCoords(Direction::left, speed);
}

void PlatformPlayerAnimator::goRight(f32 speed)
{
    updateCoords(Direction::right, speed);
}

void PlatformPlayerAnimator::goForward(f32 speed)
{
    updateCoords(Direction::up, speed);
}

void PlatformPlayerAnimator::goBackward(f32 speed)
{
    updateCoords(Direction::down, speed);
}

void PlatformPlayerAnimator::updateCoords(f32 angle, f32 speed)
{
    if (player->getAttachedCamera() != nullptr) {
        f32 x = cos(core::degToRad(player->getAttachedCamera()->getNode()->getRotation().Y) + angle);
        f32 z = sin(core::degToRad(player->getAttachedCamera()->getNode()->getRotation().Y) + angle);

        player->getNode()->setPosition(core::vector3df(
            player->getNode()->getPosition().X + ((x * -1) * (speed / 32.0f)),
            player->getNode()->getPosition().Y,
            player->getNode()->getPosition().Z + (z * (speed / 32.0f))
        ));
    }
}

scene::ISceneNodeAnimator* PlatformPlayerAnimator::createClone(scene::ISceneNode* node, scene::ISceneManager* newManager)
{
    PlatformPlayerAnimator* newAnimator = new PlatformPlayerAnimator(player);

    return newAnimator;
}

PlatformPlayerAnimator::~PlatformPlayerAnimator()
{

}
