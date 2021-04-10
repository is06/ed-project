#include "Controller.h"

Controller::Controller(EventManager* eventManager)
{
    keyboard = new Keyboard(eventManager);
}

f32 Controller::getCameraXRotation()
{
    if (keyboard->pressed(KEY_KEY_J)) {
        return -100.0f;
    }
    if (keyboard->pressed(KEY_KEY_L)) {
        return 100.0f;
    }

    return 0.0f;
}

f32 Controller::getCameraYRotation()
{
    if (keyboard->pressed(KEY_KEY_I)) {
        return 100.0f;
    }
    if (keyboard->pressed(KEY_KEY_K)) {
        return -100.0f;
    }

    return 0.0f;
}

Controller::~Controller()
{
    delete keyboard;
}