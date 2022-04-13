#pragma once

#include <stdint.h>
#include <gmock/gmock.h>

#define OUTPUT 0x1

void pinMode(uint8_t, uint8_t);
void analogWrite(uint8_t, uint8_t);
int analogRead(uint8_t);
uint32_t millis();

class ArduinoMock
{
public:
    ArduinoMock(){};

    MOCK_METHOD(void, pinMode, (uint8_t, uint8_t));
    MOCK_METHOD(void, analogWrite, (uint8_t, uint8_t));
    MOCK_METHOD(int, analogRead, (uint8_t));
    MOCK_METHOD(uint32_t, millis, ());
};

ArduinoMock *arduinoMockInstance();
void releaseArduinoMock();
