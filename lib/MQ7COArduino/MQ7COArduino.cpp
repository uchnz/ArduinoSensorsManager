#include <MQ7COArduino.h>
#include <Arduino.h>

MQ7COArduino::MQ7COArduino()
{
    _signalPIN = 0;
    _signalHeaterPIN = 0;
}

MQ7COArduino::MQ7COArduino(uint8_t signalPIN, uint8_t signalHeaterPIN)
{
    _signalPIN = signalPIN;
    _signalHeaterPIN = signalHeaterPIN;
}

void MQ7COArduino::init()
{
    pinMode(_signalPIN, OUTPUT);
    pinMode(_signalHeaterPIN, OUTPUT);
}

void MQ7COArduino::requestCurrentMeasurement()
{
}

uint8_t MQ7COArduino::getNumberOfConnectedSensors()
{
    return 1;
}

float MQ7COArduino::getCurrentMeasurementByID(uint8_t id)
{
    return analogRead(_signalPIN);
}
