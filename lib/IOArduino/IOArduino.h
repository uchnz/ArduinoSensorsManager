#pragma once
#include <Arduino.h>
#include <IIO.h>

class IOArduino : public IIO
{
protected:
    uint8_t _signalPIN;

public:
    IOArduino(uint8_t pin, uint8_t type);
    int read() = 0;
};

class AnalogIOArduino : public IOArduino
{
public:
    AnalogIOArduino(uint8_t pin, uint8_t type);
    int read() override;
};

class DigitalIOArduino : public IOArduino
{
public:
    DigitalIOArduino(uint8_t pin, uint8_t type);
    int read() override;
};
