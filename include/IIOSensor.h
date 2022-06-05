#pragma once
#include <stdint.h>

class IIOSensor
{
public:
    virtual bool init() = 0;
    virtual double read(uint8_t id = 0) = 0;
    virtual uint8_t getTotalSensors() = 0;
};
