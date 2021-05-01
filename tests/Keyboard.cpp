#include <catch2/catch.hpp>
#include "../src/Keyboard.h"
  
class EventManagerMock : public EventManager
{
public:
    void setKeyPressed(EKEY_CODE code)
    {
        keyDown[code] = true;
    }
    void releaseAllKeys()
    {
        for (int i = 0; i <= KEY_KEY_CODES_COUNT; i++) {
            keyDown[i] = false;
        }
    }
};

auto eventManagerMock = new EventManagerMock();
auto keyboard = new Keyboard(eventManagerMock);

SCENARIO("Keyboard Axis")
{
    GIVEN("isKeyDown returns true when key pressed is KEY_LEFT")
    {
        eventManagerMock->releaseAllKeys();
        eventManagerMock->setKeyPressed(KEY_LEFT);

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
        eventManagerMock->releaseAllKeys();
        eventManagerMock->setKeyPressed(KEY_RIGHT);

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
        eventManagerMock->releaseAllKeys();
        eventManagerMock->setKeyPressed(KEY_UP);

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
        eventManagerMock->releaseAllKeys();
        eventManagerMock->setKeyPressed(KEY_DOWN);

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

    GIVEN("isKeyDown returns true when key pressed are KEY_LEFT and KEY_UP")
    {
        eventManagerMock->releaseAllKeys();
        eventManagerMock->setKeyPressed(KEY_LEFT);
        eventManagerMock->setKeyPressed(KEY_UP);

        WHEN("getXAxis is called")
        {
            THEN("returned value is -87")
            {
                REQUIRE(keyboard->getXAxis() == -87);
            }
        }
        WHEN("getYAxis is called")
        {
            THEN("returned value is 87")
            {
                REQUIRE(keyboard->getYAxis() == 87);
            }
        }
    }

    GIVEN("isKeyDown returns true when key pressed are KEY_RIGHT and KEY_UP")
    {
        eventManagerMock->releaseAllKeys();
        eventManagerMock->setKeyPressed(KEY_RIGHT);
        eventManagerMock->setKeyPressed(KEY_UP);

        WHEN("getXAxis is called")
        {
            THEN("returned value is 87")
            {
                REQUIRE(keyboard->getXAxis() == 87);
            }
        }
        WHEN("getYAxis is called")
        {
            THEN("returned value is 87")
            {
                REQUIRE(keyboard->getYAxis() == 87);
            }
        }
    }

    GIVEN("isKeyDown returns true when key pressed are KEY_LEFT and KEY_DOWN")
    {
        eventManagerMock->releaseAllKeys();
        eventManagerMock->setKeyPressed(KEY_LEFT);
        eventManagerMock->setKeyPressed(KEY_DOWN);

        WHEN("getXAxis is called")
        {
            THEN("returned value is -87")
            {
                REQUIRE(keyboard->getXAxis() == -87);
            }
        }
        WHEN("getYAxis is called")
        {
            THEN("returned value is -87")
            {
                REQUIRE(keyboard->getYAxis() == -87);
            }
        }
    }

    GIVEN("isKeyDown returns true when key pressed are KEY_RIGHT and KEY_DOWN")
    {
        eventManagerMock->releaseAllKeys();
        eventManagerMock->setKeyPressed(KEY_RIGHT);
        eventManagerMock->setKeyPressed(KEY_DOWN);

        WHEN("getXAxis is called")
        {
            THEN("returned value is 87")
            {
                REQUIRE(keyboard->getXAxis() == 87);
            }
        }
        WHEN("getYAxis is called")
        {
            THEN("returned value is -87")
            {
                REQUIRE(keyboard->getYAxis() == -87);
            }
        }
    }
}
