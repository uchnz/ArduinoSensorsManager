#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <MultiLevelArduino.h>

class MultiPortTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

class MockIO : public IIOSensor
{
public:
    MOCK_METHOD(bool, init, (), (override));
    MOCK_METHOD(double, read, (uint8_t), (override));
    MOCK_METHOD(uint8_t, getTotalSensors, (), (override));
};

class MockTimer : public ITimer
{
public:
    MOCK_METHOD(bool, isReadyForNextRead, (), (override));
    MOCK_METHOD(void, setReadingInterval, (uint16_t), (override));
};