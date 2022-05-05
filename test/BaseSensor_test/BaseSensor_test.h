#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <BaseSensor.h>

class BaseSensorTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

// class MockBaseSensor : public BaseSensor
// {
// public:
//     MockBaseSensor(const char *name, IIO &io);
// };

// MockBaseSensor::MockBaseSensor(const char *name, IIO &io)
//     : BaseSensor(name, io)
// {
// }

class MockIO : public IIO
{
public:
    MOCK_METHOD(bool, init, (), (override));
    MOCK_METHOD(double, read, (uint8_t), (override));
    MOCK_METHOD(uint8_t, getTotalSensors, (), (override));
};

class MockTimer : public ITimer // TimerArduino
{
public:
    MOCK_METHOD(bool, isReadyForNextRead, (), (override));
    MOCK_METHOD(void, setReadingInterval, (uint16_t), (override));
};