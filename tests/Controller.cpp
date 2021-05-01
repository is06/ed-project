#include <catch2/catch.hpp>
#include "../src/Controller.h"

class KeyboardMock : public Keyboard
{
public:
    KeyboardMock() : Keyboard(new EventManager()) {}
    void setPressedKey(EKEY_CODE code)
    {
        isPressedKeyCode = code;
    }
    bool isPressed(EKEY_CODE code, bool once = false)
    {
        return code == isPressedKeyCode;
    }
    s8 getXAxis() {
        return 42;
    }
    s8 getYAxis() {
        return 13;
    }
private:
    EKEY_CODE isPressedKeyCode;
};

auto keyboardMock = new KeyboardMock();
auto controller = new Controller(keyboardMock);

SCENARIO("Controller player axis")
{
    GIVEN("keyboard isPressed with KEY_LEFT returns true")
    {
        keyboardMock->setPressedKey(KEY_LEFT);

        WHEN("getPlayerXAxis is called")
        {
            THEN("returned value is 42")
            {
                REQUIRE(controller->getPlayerXAxis() == 42);
            }
        }
        WHEN("getPlayerYAxis is called")
        {
            THEN("returned value is 0")
            {
                REQUIRE(controller->getPlayerYAxis() == 0);
            }
        }
    }

    GIVEN("keyboard isPressed with KEY_RIGHT returns true")
    {
        keyboardMock->setPressedKey(KEY_RIGHT);

        WHEN("getPlayerXAxis is called")
        {
            THEN("returned value is 42")
            {
                REQUIRE(controller->getPlayerXAxis() == 42);
            }
        }
        WHEN("getPlayerYAxis is called")
        {
            THEN("returned value is 0")
            {
                REQUIRE(controller->getPlayerYAxis() == 0);
            }
        }
    }

    GIVEN("keyboard isPressed with KEY_UP returns true")
    {
        keyboardMock->setPressedKey(KEY_UP);

        WHEN("getPlayerXAxis is called")
        {
            THEN("returned value is 0")
            {
                REQUIRE(controller->getPlayerXAxis() == 0);
            }
        }
        WHEN("getPlayerYAxis is called")
        {
            THEN("returned value is 13")
            {
                REQUIRE(controller->getPlayerYAxis() == 13);
            }
        }
    }

    GIVEN("keyboard isPressed with KEY_DOWN returns true")
    {
        keyboardMock->setPressedKey(KEY_DOWN);

        WHEN("getPlayerXAxis is called")
        {
            THEN("returned value is 0")
            {
                REQUIRE(controller->getPlayerXAxis() == 0);
            }
        }
        WHEN("getPlayerYAxis is called")
        {
            THEN("returned value is 13")
            {
                REQUIRE(controller->getPlayerYAxis() == 13);
            }
        }
    }
}
