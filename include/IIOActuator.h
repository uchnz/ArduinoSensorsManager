#pragma once
#include <stdint.h>

class IIOActuator
{
public:
    virtual bool init() = 0;
    virtual bool write(uint8_t value) = 0;
    // virtual uint8_t getState(char *) = 0;
};
