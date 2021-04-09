#include "EventManager.h"

EventManager::EventManager() : IEventReceiver()
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i++) {
        keyDown[i] = false;
    }
}

bool EventManager::OnEvent(const SEvent& event)
{
    if (event.EventType == EET_KEY_INPUT_EVENT) {
        keyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
    }

    return false;
}

bool EventManager::isKeyDown(EKEY_CODE code)
{
    return keyDown[code];
}
