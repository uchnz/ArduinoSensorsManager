#include <IOArduino.h>

IOArduino::IOArduino(uint8_t pin, uint8_t type)
{
    _signalPIN = pin;
    pinMode(_signalPIN, type);
}

AnalogIOArduino::AnalogIOArduino(uint8_t pin, uint8_t type)
    : IOArduino(pin, type)
{
}

int AnalogIOArduino::read()
{
    return analogRead(_signalPIN);
}

DigitalIOArduino::DigitalIOArduino(uint8_t pin, uint8_t type)
    : IOArduino(pin, type)
{
}

int DigitalIOArduino::read()
{
    return digitalRead(_signalPIN);
}