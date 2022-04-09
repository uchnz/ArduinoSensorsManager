#pragma once

#include <ISensor.h>

// sensor measurements vary in between 0 to 75% of voltage:
// 0–300: dry soil
// 300–600: wet soil;
// 600–750: sensor in water.
// see for detail: http://wiki.amperka.ru/products:sensor-soil-moisture-resistive?ysclid=l1f6io8bng

class RaindropsArduino : public ISensor
{
private:
    uint8_t _signalPIN;

public:
    RaindropsArduino();
    RaindropsArduino(uint8_t signalPIN);
    void init();

    void requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    float getCurrentMeasurementByID(uint8_t id = 0) override;
};