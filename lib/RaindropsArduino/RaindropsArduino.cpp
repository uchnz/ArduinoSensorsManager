#include <RaindropsArduino.h>
#include <Arduino.h>

RaindropsArduino::RaindropsArduino()
{
    _signalPIN = 0;
}

RaindropsArduino::RaindropsArduino(uint8_t signalPIN)
{
    _signalPIN = signalPIN;
}

void RaindropsArduino::init()
{
    pinMode(_signalPIN, OUTPUT);
}

void RaindropsArduino::requestCurrentMeasurement()
{
}

uint8_t RaindropsArduino::getNumberOfConnectedSensors()
{
    return 1;
}

float RaindropsArduino::getCurrentMeasurementByID(uint8_t id)
{
    return analogRead(_signalPIN);
}