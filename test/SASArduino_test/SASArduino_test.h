#include <gtest/gtest.h>
#include <IOArduino.h>

class SASArduinoTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

class MockAnalogIOArduino : public AnalogIOArduino
{
public:
    MockAnalogIOArduino(uint8_t pin, uint8_t type);
};

MockAnalogIOArduino::MockAnalogIOArduino(uint8_t pin, uint8_t type)
    : AnalogIOArduino(pin, type)
{
}

class MockDigitalIOArduino : public DigitalIOArduino
{
public:
    MockDigitalIOArduino(uint8_t pin, uint8_t type);
};

MockDigitalIOArduino::MockDigitalIOArduino(uint8_t pin, uint8_t type)
    : DigitalIOArduino(pin, type)
{
}