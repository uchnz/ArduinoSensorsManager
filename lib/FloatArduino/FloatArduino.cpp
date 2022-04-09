#include <FloatArduino.h>
#include <Arduino.h>

FloatArduino::FloatArduino()
{
    _signalPIN = 0;
}

FloatArduino::FloatArduino(uint8_t signalPIN)
{
    _signalPIN = signalPIN;
}

void FloatArduino::init()
{
    pinMode(_signalPIN, INPUT_PULLUP);
}

void FloatArduino::requestCurrentMeasurement()
{
}

uint8_t FloatArduino::getNumberOfConnectedSensors()
{
    return 1;
}

float FloatArduino::getCurrentMeasurementByID(uint8_t id)
{
    return digitalRead(_signalPIN);
}