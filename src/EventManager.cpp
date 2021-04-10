#include "EventManager.h"

EventManager::EventManager() : IEventReceiver()
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i++) {
        keyDown[i] = false;
        keyDownOnce[i] = false;
    }
}

bool EventManager::OnEvent(const SEvent& event)
{
    switch (event.EventType) {
        case EET_KEY_INPUT_EVENT:
            keyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
            break;
        default:
            break;
    }

    return false;
}

bool EventManager::isKeyDown(EKEY_CODE code)
{
    return keyDown[code];
}

bool EventManager::isKeyDownOnce(EKEY_CODE code)
{
    if (!keyDownOnce[code]) {
        if (isKeyDown(code)) {
            keyDownOnce[code] = true;
            return true;
        }
    } else {
        if (!isKeyDown(code)) {
            keyDownOnce[code] = false;
        }
    }

    return false;
}
