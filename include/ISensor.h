#pragma once
#include <stdint.h>

class ISensor
{
public:
    virtual ~ISensor() = default;

    virtual void requestCurrentMeasurement() = 0;
    virtual uint8_t getNumberOfConnectedSensors() = 0;
    virtual float getCurrentMeasurementByID(uint8_t id) = 0;
};
