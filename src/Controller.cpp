#include "Controller.h"

Controller::Controller(EventManager* eventManager)
{
    keyboard = new Keyboard(eventManager);
}

bool Controller::isActionPerformed(ControlAction action, bool once)
{
    switch (action) {
        case CONTROL_ACTION_JUMP:
            return keyboard->isPressed(KEY_KEY_V, once);
            break;
        default:
            break;
    }

    return false;
}

f32 Controller::getPlayerXAxis()
{
    if (keyboard->isPressed(KEY_LEFT) || keyboard->isPressed(KEY_RIGHT)) {
        return keyboard->getXAxis();
    }

    return 0.0f;
}

f32 Controller::getPlayerYAxis()
{
    if (keyboard->isPressed(KEY_UP) || keyboard->isPressed(KEY_DOWN)) {
        return keyboard->getYAxis();
    }

    return 0.0f;
}

f32 Controller::getCameraXAxis()
{
    if (keyboard->isPressed(KEY_KEY_J)) {
        return -100.0f;
    }
    if (keyboard->isPressed(KEY_KEY_L)) {
        return 100.0f;
    }

    return 0.0f;
}

f32 Controller::getCameraYAxis()
{
    if (keyboard->isPressed(KEY_KEY_I)) {
        return 100.0f;
    }
    if (keyboard->isPressed(KEY_KEY_K)) {
        return -100.0f;
    }

    return 0.0f;
}

Keyboard* Controller::getKeyboard()
{
    return keyboard;
}

Controller::~Controller()
{
    delete keyboard;
}
