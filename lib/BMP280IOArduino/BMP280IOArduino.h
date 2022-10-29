#pragma once
#include <IIOSensor.h>
#include <iarduino_Pressure_BMP.h>

namespace bmpio_nm
{
    const double UNINITIALIZED_SENSOR_VALUE = 0xFFFFFFFF;
    const uint8_t TEMPERATURE_ID = 0;
    const uint8_t PRESSURE_ID = 1;
    const uint8_t ALTITUDE_ID = 2;
    const uint8_t TOTAL_SENSORS_ON_BUS = 3;
}

class BMP280IOArduino : public IIOSensor
{
private:
    iarduino_Pressure_BMP &_bmp;
    bool _initCompleted;

public:
    BMP280IOArduino(iarduino_Pressure_BMP &bmp);
    // bool isInitCompleted();

    // IIOSensor
    virtual bool isInitCompleted() override;
    virtual bool init() override;
    virtual double read(uint8_t id = 0) override;
    virtual uint8_t getTotalSensors() override;
};