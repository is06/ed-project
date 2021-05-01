#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <irrlicht/irrlicht.h>

using namespace irr;

class EventManager : public IEventReceiver
{
public:
    EventManager();

    virtual bool OnEvent(const SEvent& event);
    virtual bool isKeyDown(EKEY_CODE code);
    virtual bool isKeyDownOnce(EKEY_CODE code);

protected:
    bool keyDown[KEY_KEY_CODES_COUNT];
    bool keyDownOnce[KEY_KEY_CODES_COUNT];
};

#endif