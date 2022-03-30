#include <gmock/gmock.h>
#include <ISensor.h>

class MockIDallas : public ISensor
{
public:
    MOCK_METHOD(void, requestCurrentTemperatures, (), (override));
    MOCK_METHOD(uint8_t, getNumberOfConnectedSensors, (), (override));
    MOCK_METHOD(float, getTemperatureByID, (uint8_t), (override));
};
