#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <SimpleActuator.h>

class SimpleActuatorTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
};

class MockIOActuator : public IIOActuator
{
public:
    MOCK_METHOD(bool, init, (), (override));
    MOCK_METHOD(bool, write, (uint8_t), (override));
    // MOCK_METHOD(uint8_t, getState, (char *), (override));
};
