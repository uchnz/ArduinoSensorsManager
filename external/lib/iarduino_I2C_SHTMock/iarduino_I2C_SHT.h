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
    MOCK_METHOD(float, getTem, ());
    MOCK_METHOD(float, getHum, ());
};
// bool begin(void);
// bool reset(void);
// bool changeAddress(uint8_t);
// // uint8_t getAddress(void);
// uint8_t getVersion(void);
// bool getPullI2C(void);
// bool setPullI2C(bool = true);
// float getHum(void);
// bool getTemChanged(void);
// bool getHumChanged(void);
// bool setTemChange(float);
// bool setHumChange(float);
// bool setPeriod(float);

// MOCK_METHOD(void, analogWrite, (uint8_t, uint8_t));
// MOCK_METHOD(int, analogRead, (uint8_t));
// MOCK_METHOD(uint32_t, millis, ());
