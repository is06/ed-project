#include "Keyboard.h"
#include "Direction.h"

Keyboard::Keyboard(EventManager* eventManager)
{
    this->eventManager = eventManager;
}

bool Keyboard::pressed(EKEY_CODE code, bool once)
{
    if (once) {
        return eventManager->isKeyDownOnce(code);
    }
    return eventManager->isKeyDown(code);
}

s8 Keyboard::getXAxis()
{
    if (eventManager->isKeyDown(KEY_LEFT)) {
        if (eventManager->isKeyDown(KEY_UP)) {
            directionAngle = Direction::upLeft;
            return -87;
        } else if (eventManager->isKeyDown(KEY_DOWN)) {
            directionAngle = Direction::downLeft;
            return -87;
        }
        directionAngle = Direction::left;
        return -127;
    } else if (eventManager->isKeyDown(KEY_RIGHT)) {
        if (eventManager->isKeyDown(KEY_UP)) {
            directionAngle = Direction::upRight;
            return 87;
        } else if (eventManager->isKeyDown(KEY_DOWN)) {
            directionAngle = Direction::downRight;
            return 87;
        }
        directionAngle = Direction::right;
        return 127;
    }

    return 0;
}

s8 Keyboard::getYAxis()
{
    if (eventManager->isKeyDown(KEY_UP)) {
        if (eventManager->isKeyDown(KEY_LEFT)) {
            directionAngle = Direction::upLeft;
            return 87;
        } else if (eventManager->isKeyDown(KEY_RIGHT)) {
            directionAngle = Direction::upRight;
            return 87;
        }
        directionAngle = Direction::up;
        return 127;
    } else if (eventManager->isKeyDown(KEY_DOWN)) {
        if (eventManager->isKeyDown(KEY_LEFT)) {
            directionAngle = Direction::downLeft;
            return -87;
        } else if (eventManager->isKeyDown(KEY_RIGHT)) {
            directionAngle = Direction::downRight;
            return -87;
        }
        directionAngle = Direction::down;
        return -127;
    }

    return 0;
}

f32 Keyboard::getDirectionAngle()
{
    return directionAngle;
}
