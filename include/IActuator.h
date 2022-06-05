#pragma once
#include <stdint.h>

class IActuator
{
public:
    virtual bool execute(const char *command) = 0;
    virtual uint8_t getName(char *name) = 0;
    virtual uint8_t getState(char *) = 0;
};