#pragma once

#include <DallasTemperature.h>

#include "ISensor.h"

class DallasArduino : public ISensor
{
private:
    OneWire _oneWire;
    DallasTemperature _sensors;
    void showNumberOfFoundSensors();
    void showParasitePowerMode();
    void showSensorsAddresses();
    String getAddressString(DeviceAddress deviceAddress);

public:
    DallasArduino();
    DallasArduino(uint8_t sensorPIN);
    void init();
    void setSensorsPrecision(int precision);

    // ISensor
    void requestCurrentTemperatures() override;
    uint8_t getNumberOfConnectedSensors() override;
    float getTemperatureByID(uint8_t) override;
};
