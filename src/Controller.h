#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "EventManager.h"
#include "Keyboard.h"

class EventManager;
class Keyboard;

enum ControlAction
{
    ACTION_JUMP,
    ACTION_ATTACK
};

class Controller
{
public:
    Controller(EventManager* eventManager);
    ~Controller();

    bool isActionPerformed(ControlAction action, bool once = false);

    f32 getCameraXRotation();
    f32 getCameraYRotation();

private:
    Keyboard* keyboard;
};

#endif
