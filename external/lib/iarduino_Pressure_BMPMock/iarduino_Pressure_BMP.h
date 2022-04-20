#pragma once

#include <stdint.h>
#include <gmock/gmock.h>

class iarduino_Pressure_BMP
{
private:
    uint8_t _address;

public:
    float temperature;
    float pressure;
    float altitude;

    iarduino_Pressure_BMP(uint8_t address){};

    MOCK_METHOD(bool, begin, ());
    // MOCK_METHOD(uint8_t, getAddress, ());
    MOCK_METHOD(bool, read, ());
    MOCK_METHOD(bool, read, (uint8_t)); // duplicate read because an argument is optional
};
