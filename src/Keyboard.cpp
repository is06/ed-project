#include "Keyboard.h"

Keyboard::Keyboard(EventManager* eventManager)
{
    this->eventManager = eventManager;
}

bool Keyboard::pressed(EKEY_CODE code, bool once)
{
    if (once) {
        return eventManager->isKeyDownOnce(code);
    } else {
        return eventManager->isKeyDown(code);
    }
}

s8 Keyboard::getXAxis()
{
    if (eventManager->isKeyDown(KEY_LEFT)) {
        if (eventManager->isKeyDown(KEY_UP)) {
            // up left
            directionAngle = 135.0f;
            return -87;
        } else if (eventManager->isKeyDown(KEY_DOWN)) {
            // down left
            directionAngle = 225.0f;
            return -87;
        } else {
            // left
            directionAngle = 180.0f;
            return -127;
        }
    } else if (eventManager->isKeyDown(KEY_RIGHT)) {
        if (eventManager->isKeyDown(KEY_UP)) {
            // up right
            directionAngle = 45.0f;
            return 87;
        } else if (eventManager->isKeyDown(KEY_DOWN)) {
            // down right
            directionAngle = 315.0f;
            return 87;
        } else {
            // right
            directionAngle = 0.0f;
            return 127;
        }
    }

    return 0;
}

s8 Keyboard::getYAxis()
{
    if (eventManager->isKeyDown(KEY_UP)) {
        if (eventManager->isKeyDown(KEY_LEFT)) {
            // up left
            directionAngle = 135.0f;
            return 87;
        } else if (eventManager->isKeyDown(KEY_RIGHT)) {
            // up right
            directionAngle = 45.0f;
            return 87;
        } else {
            // up
            directionAngle = 90.0f;
            return 127;
        }
    } else if (eventManager->isKeyDown(KEY_DOWN)) {
        if (eventManager->isKeyDown(KEY_LEFT)) {
            // down left
            directionAngle = 225.0f;
            return -87;
        } else if (eventManager->isKeyDown(KEY_RIGHT)) {
            // down right
            directionAngle = 315.0f;
            return -87;
        } else {
            // down
            directionAngle = 270.0f;
            return -127;
        }
    }

    return 0;
}

f32 Keyboard::getDirectionAngle()
{
    return directionAngle;
}
