#pragma once

#include <ISensor.h>

class FloatArduino : public ISensor
{
private:
    uint8_t _signalPIN;

public:
    FloatArduino();
    FloatArduino(uint8_t signalPIN);
    void init();

    void requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    float getCurrentMeasurementByID(uint8_t id = 0) override;
};