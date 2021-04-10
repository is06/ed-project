#ifndef TP_CAMERA_H
#define TP_CAMERA_H

#include "../Camera.h"
#include "../Player.h"
#include "../../Controller.h"

class Controller;
class Player;

class TpCamera : public Camera
{
public:
    TpCamera(Player* player, Controller* controller);
    void update(f32 speed);
    ~TpCamera();

private:
    void goLeft(f32 speed);
    void goRight(f32 speed);
    void goNear(f32 speed);
    void goFar(f32 speed);

    Player* player;
    Controller* controller;
    f32 rotation;
    f32 height;
    f32 distance;

    static constexpr f32 ROTATION_SPEED = 60.0f;
    static constexpr f32 DISTANCE_SPEED = 50.0f;
    static constexpr f32 MAX_FAR = 10.0f;
    static constexpr f32 MIN_NEAR = 5.0f;
    static constexpr f32 MAX_HEIGHT = 5.0f;
    static constexpr f32 MIN_HEIGHT = 1.0f;
};

#endif
