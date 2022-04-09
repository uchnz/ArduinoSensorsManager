#pragma once

#include <ISensor.h>

enum VOLTAGE
{
    HIGH_5_0 = 255,
    LOW_1_4 = 72
};

class MQ7COArduino : public ISensor
{
private:
    uint8_t _signalPIN;
    uint8_t _signalHeaterPIN;

public:
    MQ7COArduino();
    MQ7COArduino(uint8_t signalPIN, uint8_t signalHeaterPIN);

    void init();

    void requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    float getCurrentMeasurementByID(uint8_t id = 0) override;
};
