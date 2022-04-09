#include <MoisureRArduino.h>
#include <Arduino.h>

MoisureRArduino::MoisureRArduino()
{
    _signalPIN = 0;
}

MoisureRArduino::MoisureRArduino(uint8_t signalPIN)
{
    _signalPIN = signalPIN;
}

void MoisureRArduino::init()
{
    pinMode(_signalPIN, OUTPUT);
}

void MoisureRArduino::requestCurrentMeasurement()
{
}

uint8_t MoisureRArduino::getNumberOfConnectedSensors()
{
    return 1;
}

float MoisureRArduino::getCurrentMeasurementByID(uint8_t id)
{
    return analogRead(_signalPIN);
}