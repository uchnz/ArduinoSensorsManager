#pragma once

#include <stdint.h>
#include <gmock/gmock.h>

#define INPUT 0x0
#define INPUT_PULLUP 0x2
#define OUTPUT 0x1

#define HIGH 0x1
#define LOW 0x0

void pinMode(uint8_t, uint8_t);
void analogWrite(uint8_t, uint8_t);
int analogRead(uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);

uint32_t millis();

class ArduinoMock
{
public:
    ArduinoMock(){};

    MOCK_METHOD(void, pinMode, (uint8_t, uint8_t));
    MOCK_METHOD(void, analogWrite, (uint8_t, uint8_t));
    MOCK_METHOD(int, analogRead, (uint8_t));
    MOCK_METHOD(void, digitalWrite, (uint8_t, uint8_t));
    MOCK_METHOD(int, digitalRead, (uint8_t));
    MOCK_METHOD(uint32_t, millis, ());
};

ArduinoMock *arduinoMockInstance();
void releaseArduinoMock();

#define PIN_SPI_SS (53)
#define PIN_SPI_MOSI (51)
#define PIN_SPI_MISO (50)
#define PIN_SPI_SCK (52)

static const uint8_t SS = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK = PIN_SPI_SCK;

#define PIN_WIRE_SDA (20)
#define PIN_WIRE_SCL (21)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN 13

#define PIN_A0 (54)
#define PIN_A1 (55)
#define PIN_A2 (56)
#define PIN_A3 (57)
#define PIN_A4 (58)
#define PIN_A5 (59)
#define PIN_A6 (60)
#define PIN_A7 (61)
#define PIN_A8 (62)
#define PIN_A9 (63)
#define PIN_A10 (64)
#define PIN_A11 (65)
#define PIN_A12 (66)
#define PIN_A13 (67)
#define PIN_A14 (68)
#define PIN_A15 (69)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;
static const uint8_t A8 = PIN_A8;
static const uint8_t A9 = PIN_A9;
static const uint8_t A10 = PIN_A10;
static const uint8_t A11 = PIN_A11;
static const uint8_t A12 = PIN_A12;
static const uint8_t A13 = PIN_A13;
static const uint8_t A14 = PIN_A14;
static const uint8_t A15 = PIN_A15;