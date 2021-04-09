#ifndef GAME_EXCEPTION_H
#define GAME_EXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class GameException : public exception
{
public:
    GameException(const string& message = "") throw()
    {
        this->message = message;
    }
    virtual ~GameException() {}
    virtual const char* what() const throw()
    {
        return message.c_str();
    }
private:
    string message;
};

#endif
