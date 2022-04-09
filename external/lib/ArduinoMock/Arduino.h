#pragma once

#include <stdint.h>
#include <gmock/gmock.h>

#define OUTPUT 0x1

void pinMode(uint8_t, uint8_t);
int analogRead(uint8_t);

class ArduinoMock
{
public:
    ArduinoMock(){};

    MOCK_METHOD2(pinMode, void(uint8_t, uint8_t));
};

ArduinoMock *arduinoMockInstance();
void releaseArduinoMock();
