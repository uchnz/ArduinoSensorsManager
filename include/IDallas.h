#pragma once
#include <stdint.h>

// namespace sensor_manager
// {
class IDallas
{
public:
    virtual ~IDallas() = default;

    virtual void requestCurrentTemperatures() = 0;
    virtual uint8_t getNumberOfConnectedSensors() = 0;
    virtual float getTemperatureByID(uint8_t id) = 0;
    // virtual void getStringAddressByID(uint8_t id, char *address) = 0;
};
// }