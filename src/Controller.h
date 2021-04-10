#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "EventManager.h"
#include "Keyboard.h"

class EventManager;
class Keyboard;

class Controller
{
public:
    Controller(EventManager* eventManager);
    ~Controller();

    f32 getCameraXRotation();
    f32 getCameraYRotation();

private:
    Keyboard* keyboard;
};

#endif
