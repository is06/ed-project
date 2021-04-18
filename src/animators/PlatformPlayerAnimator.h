#ifndef PLATFORM_PLAYER_ANIMATOR_H
#define PLATFORM_PLAYER_ANIMATOR_H

#include <irrlicht/irrlicht.h>
#include "../entities/Player.h"

using namespace irr;

class Player;

class PlatformPlayerAnimator : public scene::ISceneNodeAnimator
{
public:
    PlatformPlayerAnimator(Player* player);
    virtual ~PlatformPlayerAnimator();

    virtual void animateNode(scene::ISceneNode* node, u32 timeMs);
    virtual scene::ISceneNodeAnimator* createClone(scene::ISceneNode* node, scene::ISceneManager* newManager = nullptr);

    void jump();
    void run(f32 angle);

private:
    Player* player;
};

#endif
