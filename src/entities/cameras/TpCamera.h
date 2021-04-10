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
    void update();
    ~TpCamera();

    void goLeft(f32 speed);
    void goRight(f32 speed);
    void goNear(f32 speed);
    void goFar(f32 speed);

private:
    Player* player;
    Controller* controller;
    f32 rotation;
    f32 height;
    f32 distance;
};

#endif
