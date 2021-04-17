#ifndef PLATFORM_PLAYER_ANIMATOR_H
#define PLATFORM_PLAYER_ANIMATOR_H

#include <irrlicht/irrlicht.h>

using namespace irr;

class PlatformPlayerAnimator : public virtual scene::ISceneNodeAnimatorCollisionResponse
{
public:
    PlatformPlayerAnimator();
    virtual ~PlatformPlayerAnimator();
};

#endif
