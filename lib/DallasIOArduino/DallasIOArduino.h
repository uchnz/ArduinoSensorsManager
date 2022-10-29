#pragma once
#include <IIOSensor.h>
#include <DallasTemperature.h>

namespace dallas_nm
{
    const double UNINITIALIZED_SENSOR_VALUE = 0xFFFFFFFF;
    const uint8_t TEMPERATURE_ID = 0;
    const uint8_t HUMIDITY_ID = 1;
    const uint8_t TOTAL_SENSORS_ON_BUS = 2;
}

class DallasIOArduino : public IIOSensor
{
private:
    DallasTemperature &_dallas;
    bool _initCompleted;

public:
    DallasIOArduino(DallasTemperature &dallas);
    // bool isInitCompleted();

    // IIO
    virtual bool isInitCompleted() override;
    virtual bool init() override;
    virtual double read(uint8_t id = 0) override;
    virtual uint8_t getTotalSensors() override;
};