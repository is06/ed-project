#include "Controller.h"

Controller::Controller(EventManager* eventManager)
{
    keyboard = new Keyboard(eventManager);
}

bool Controller::isActionPerformed(ControlAction action, bool once)
{
    switch (action) {
        case CONTROL_ACTION_JUMP:
            return keyboard->pressed(KEY_KEY_V, once);
            break;
        default:
            break;
    }

    return false;
}

f32 Controller::getPlayerXAxis()
{
    if (keyboard->pressed(KEY_LEFT) || keyboard->pressed(KEY_RIGHT)) {
        return keyboard->getXAxis();
    }

    return 0.0f;
}

f32 Controller::getPlayerYAxis()
{
    if (keyboard->pressed(KEY_UP) || keyboard->pressed(KEY_DOWN)) {
        return keyboard->getYAxis();
    }

    return 0.0f;
}

f32 Controller::getCameraXAxis()
{
    if (keyboard->pressed(KEY_KEY_J)) {
        return -100.0f;
    }
    if (keyboard->pressed(KEY_KEY_L)) {
        return 100.0f;
    }

    return 0.0f;
}

f32 Controller::getCameraYAxis()
{
    if (keyboard->pressed(KEY_KEY_I)) {
        return 100.0f;
    }
    if (keyboard->pressed(KEY_KEY_K)) {
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
