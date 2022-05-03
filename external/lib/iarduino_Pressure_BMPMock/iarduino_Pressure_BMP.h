#pragma once

#include <stdint.h>
#include <gmock/gmock.h>

class iarduino_Pressure_BMP
{
private:
    uint8_t _address;

public:
    double temperature;
    double pressure;
    double altitude;

    iarduino_Pressure_BMP(uint8_t address){};

    MOCK_METHOD(bool, begin, ());
    MOCK_METHOD(bool, read, ());
    MOCK_METHOD(bool, read, (uint8_t)); // duplicate read() because an argument is optional
};
