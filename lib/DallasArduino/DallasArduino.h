#pragma once

#include <DallasTemperature.h>

#include "IDallas.h"

// namespace sensor_manager
// {

class DallasArduino : public IDallas
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

    // IDallas
    void requestCurrentTemperatures() override;
    uint8_t getNumberOfConnectedSensors() override;
    float getTemperatureByID(uint8_t) override;
    // void getStringAddressByID(uint8_t id, char *address) override;
};
// }