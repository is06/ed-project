#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "EventManager.h"
#include "Keyboard.h"

class EventManager;
class Keyboard;

enum ControlAction
{
    CONTROL_ACTION_JUMP,
    CONTROL_ACTION_ATTACK
};

class Controller
{
public:
    Controller(EventManager* eventManager);
    ~Controller();

    bool isActionPerformed(ControlAction action, bool once = false);

    f32 getPlayerXAxis();
    f32 getPlayerYAxis();
    f32 getCameraXAxis();
    f32 getCameraYAxis();

    Keyboard* getKeyboard();

private:
    Keyboard* keyboard;
};

#endif
