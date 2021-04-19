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
    void goLeft(f32 speed);
    void goRight(f32 speed);
    void goForward(f32 speed);
    void goBackward(f32 speed);

private:
    void updateCoords(f32 angle, f32 speed);

    Player* player;
};

#endif
