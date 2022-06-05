#pragma once
#include <Arduino.h>
#include <IIOSensor.h>

namespace ioarduino_nm
{
    const double UNINITIALIZED_SENSOR_VALUE = 0xFFFFFFFF;
    const uint8_t TOTAL_SENSORS_ON_BUS = 1;
}

class IOArduino : public IIOSensor
{
protected:
    uint8_t _signalPIN;
    uint8_t _signalDirection;
    bool _initCompleted;

public:
    IOArduino(uint8_t pin, uint8_t direction);

    bool isInitCompleted();

    // IIOSensor
    virtual bool init() override;
    virtual double read(uint8_t id = 0) = 0;
    virtual uint8_t getTotalSensors() override;
};

class AnalogIOArduino : public IOArduino
{
public:
    AnalogIOArduino(uint8_t pin, uint8_t direction);

    virtual double read(uint8_t id = 0) override;
};

class DigitalIOArduino : public IOArduino
{
public:
    DigitalIOArduino(uint8_t pin, uint8_t direction);

    virtual double read(uint8_t id = 0) override;
};
