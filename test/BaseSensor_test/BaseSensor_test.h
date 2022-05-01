#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <BaseSensor.h>

class ISensorTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

class MockBaseSensor : public BaseSensor
{
public:
    MockBaseSensor(const char *name, IIO &io);
};

MockBaseSensor::MockBaseSensor(const char *name, IIO &io)
    : BaseSensor(name, io)
{
}

class MockIO : public IIO
{
public:
    MOCK_METHOD(int, read, (), (override));
};

class MockTimer : public TimerArduino
{
public:
    MOCK_METHOD(bool, isReadyForNextRead, (), (override));
};