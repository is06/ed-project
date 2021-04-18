#include "PlatformPlayerAnimator.h"

PlatformPlayerAnimator::PlatformPlayerAnimator(Player* player)
{
    this->player = player;
}

void PlatformPlayerAnimator::animateNode(scene::ISceneNode* node, u32 timeMs)
{
    
}

void PlatformPlayerAnimator::run(f32 angle)
{
    
}

void PlatformPlayerAnimator::jump()
{

}

scene::ISceneNodeAnimator* PlatformPlayerAnimator::createClone(scene::ISceneNode* node, scene::ISceneManager* newManager)
{
    PlatformPlayerAnimator* newAnimator = new PlatformPlayerAnimator(player);

    return newAnimator;
}

PlatformPlayerAnimator::~PlatformPlayerAnimator()
{

}
