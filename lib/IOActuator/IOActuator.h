#pragma once
#include <Arduino.h>
#include <IIOActuator.h>

namespace ioactuator_nm
{
    // const double UNINITIALIZED_SENSOR_VALUE = 0xFFFFFFFF;
    // const uint8_t TOTAL_SENSORS_ON_BUS = 1;
}

class IOActuator : public IIOActuator
{
public:
    IOActuator(uint8_t pin);

    virtual bool init() override;
    virtual bool write(uint8_t value) override;
};