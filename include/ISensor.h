#pragma once
#include <stdint.h>

class ISensor
{
public:
    virtual ~ISensor() = default;

    virtual void requestCurrentTemperatures() = 0;
    virtual uint8_t getNumberOfConnectedSensors() = 0;
    virtual float getTemperatureByID(uint8_t id) = 0;
};
