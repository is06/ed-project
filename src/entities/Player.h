#ifndef PLAYER_H
#define PLAYER_H

#include "Model.h"
#include "World.h"
#include "cameras/TpCamera.h"
#include "../Map.h"
#include "../animators/PlatformPlayerAnimator.h"

class Map;
class PlatformPlayerAnimator;
class TpCamera;
class World;

class Player : public Model
{
public:
    Player(Map* scene);
    void update(f32 speed);
    ~Player();

    void attachWorldForCollisions(World* world);
    void attachThirdPersonCamera(TpCamera* camera);
    Map* getMap();

private:
    void goLeft(f32 speed);
    void goRight(f32 speed);
    void goForward(f32 speed);
    void goBackward(f32 speed);
    void updateCoords(f32 angle, f32 speed);

    PlatformPlayerAnimator* platformPlayerAnimator;
    TpCamera* attachedCamera;
};

#endif
