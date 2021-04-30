#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <irrlicht/irrlicht.h>

#include "EventManager.h"

using namespace irr;

class EventManager;

class Keyboard
{
public:
    Keyboard(EventManager* eventManager);
    bool isPressed(EKEY_CODE code, bool once = false);
    s8 getXAxis();
    s8 getYAxis();
    f32 getDirectionAngle();

private:
    EventManager* eventManager;
    f32 directionAngle;
};

#endif
