#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <irrlicht/irrlicht.h>

using namespace irr;

class EventManager : public IEventReceiver
{
public:
    EventManager();

    bool OnEvent(const SEvent& event);
    bool isKeyDown(EKEY_CODE code);

private:
    bool keyDown[KEY_KEY_CODES_COUNT];
};

#endif