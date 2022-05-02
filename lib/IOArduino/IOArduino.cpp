#include <IOArduino.h>

IOArduino::IOArduino(uint8_t pin, uint8_t direction)
{
    _signalPIN = pin;
    _signalDirection = direction;
    _initCompleted = false;
}

bool IOArduino::isInitCompleted()
{
    return _initCompleted;
}
bool IOArduino::init()
{
    pinMode(_signalPIN, _signalDirection);
    _initCompleted = true;

    return _initCompleted;
}

uint8_t IOArduino::getTotalSensors()
{
    return ioarduino_nm::TOTAL_SENSORS_ON_BUS;
}

///////////////////////////

AnalogIOArduino::AnalogIOArduino(uint8_t pin, uint8_t direction)
    : IOArduino(pin, direction)
{
}

double AnalogIOArduino::read(uint8_t id)
{
    if (!isInitCompleted())
        return ioarduino_nm::UNINITIALIZED_SENSOR_VALUE;

    return (double)analogRead(_signalPIN);
}

DigitalIOArduino::DigitalIOArduino(uint8_t pin, uint8_t direction)
    : IOArduino(pin, direction)
{
}

double DigitalIOArduino::read(uint8_t id)
{
    if (!isInitCompleted())
        return ioarduino_nm::UNINITIALIZED_SENSOR_VALUE;

    return (double)digitalRead(_signalPIN);
}