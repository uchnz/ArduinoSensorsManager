#pragma once

#include <DallasTemperature.h>

#include <IDallas.h>

namespace sensor_manager
{

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
        void init() override;
        void setSensorsPrecision(int precision) override;
        void requestCurrentTemperatures() override;
        uint8_t getNumberOfSensors() override;
        float getTemperatureByID(uint8_t) override;
        void getStringAddressByID(uint8_t id, char *address) override;
    };
}