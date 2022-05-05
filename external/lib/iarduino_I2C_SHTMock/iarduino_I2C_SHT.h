#pragma once

#include <stdint.h>
#include <gmock/gmock.h>

class iarduino_I2C_SHT
{
private:
    uint8_t _address;

public:
    iarduino_I2C_SHT(uint8_t address){};

    MOCK_METHOD(bool, begin, ());
    MOCK_METHOD(uint8_t, getAddress, ());
    MOCK_METHOD(double, getTem, ());
    MOCK_METHOD(double, getHum, ());
};
