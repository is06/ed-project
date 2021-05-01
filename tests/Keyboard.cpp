#include <catch2/catch.hpp>
#include "../src/Keyboard.h"
  
class EventManagerMock : public EventManager
{
public:
    EKEY_CODE keysPressed;
    bool isKeyDown(EKEY_CODE code) { return code == keysPressed; }
};

auto eventManagerMock = new EventManagerMock();
auto keyboard = new Keyboard(eventManagerMock);

SCENARIO("Keyboard Axis")
{
    GIVEN("isKeyDown returns true when keysPressed equals KEY_LEFT")
    {
        eventManagerMock->keysPressed = KEY_LEFT;

        WHEN("getXAxis is called")
        {
            THEN("returned value is -127")
            {
                REQUIRE(keyboard->getXAxis() == -127);
            }
        }
        WHEN("getYAxis is called")
        {
            THEN("returned value is 0")
            {
                REQUIRE(keyboard->getYAxis() == 0);
            }
        }
    }
    
    GIVEN("isKeyDown returns true when keysPressed equals KEY_RIGHT")
    {
        eventManagerMock->keysPressed = KEY_RIGHT;

        WHEN("getXAxis is called")
        {
            THEN("returned value is 127")
            {
                REQUIRE(keyboard->getXAxis() == 127);
            }
        }
        WHEN("getYAxis is called")
        {
            THEN("returned value is 0")
            {
                REQUIRE(keyboard->getYAxis() == 0);
            }
        }
    }
    
    GIVEN("isKeyDown returns true when keysPressed equals KEY_UP")
    {
        eventManagerMock->keysPressed = KEY_UP;

        WHEN("getXAxis is called")
        {
            THEN("returned value is 0")
            {
                REQUIRE(keyboard->getXAxis() == 0);
            }
        }
        WHEN("getYAxis is called")
        {
            THEN("returned value is 127")
            {
                REQUIRE(keyboard->getYAxis() == 127);
            }
        }
    }
    GIVEN("isKeyDown returns true when keysPressed equals KEY_DOWN")
    {
        eventManagerMock->keysPressed = KEY_DOWN;

        WHEN("getXAxis is called")
        {
            THEN("returned value is 0")
            {
                REQUIRE(keyboard->getXAxis() == 0);
            }
        }
        WHEN("getYAxis is called")
        {
            THEN("returned value is -127")
            {
                REQUIRE(keyboard->getYAxis() == -127);
            }
        }
    }
}
