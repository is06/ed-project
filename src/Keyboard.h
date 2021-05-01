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
    virtual ~Keyboard();
    virtual bool isPressed(EKEY_CODE code, bool once = false);
    virtual s8 getXAxis();
    virtual s8 getYAxis();
    virtual f32 getDirectionAngle();

private:
    EventManager* eventManager;
    f32 directionAngle;
};

#endif
