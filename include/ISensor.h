#pragma once
#include <stdint.h>

class ISensor
{
public:
    // virtual void requestCurrentMeasurement() = 0;
    virtual bool requestCurrentMeasurement() = 0;
    virtual uint8_t getNumberOfConnectedSensors() = 0;
    virtual double getCurrentMeasurementByID(uint8_t id) = 0;
    virtual uint8_t getName(char *name) = 0;
};