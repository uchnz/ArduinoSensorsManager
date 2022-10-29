#ifndef __TESTS__
#include <LibPrintf.h>
#endif

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

    // printf("in AnalogIO");

    // return 125;
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

MultiPortIOArduino::MultiPortIOArduino(IIOSensor **ios, uint8_t num)
{
    _ios = ios;
    _totalSensors = num;
    _initCompleted = false;
}

bool MultiPortIOArduino::isInitCompleted()
{
    return _initCompleted;
}
bool MultiPortIOArduino::init()
{
    for (uint8_t i = 0; i < _totalSensors; i++)
        _ios[i]->init();

    _initCompleted = true;
    return _initCompleted;
}
double MultiPortIOArduino::read(uint8_t id)
{
    if (!isInitCompleted())
        return ioarduino_nm::UNINITIALIZED_SENSOR_VALUE;

    if (id < _totalSensors)
        return _ios[id]->read();

    return ioarduino_nm::UNINITIALIZED_SENSOR_VALUE;
}

uint8_t MultiPortIOArduino::getTotalSensors()
{
    return _totalSensors;
}
