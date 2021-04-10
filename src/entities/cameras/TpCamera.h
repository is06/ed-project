#ifndef TP_CAMERA_H
#define TP_CAMERA_H

#include "../Camera.h"
#include "../Player.h"

class Player;

class TpCamera : public Camera
{
public:
    TpCamera(Player* player);
    void update();
    ~TpCamera();

    void goLeft(f32 speed);
    void goRight(f32 speed);
    void goNear(f32 speed);
    void goFar(f32 speed);

private:
    Player* player;
    f32 rotation;
    f32 height;
    f32 distance;
};

#endif
