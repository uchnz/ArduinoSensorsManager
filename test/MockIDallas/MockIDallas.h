#include <gmock/gmock.h>
#include <IDallas.h>

class MockIDallas : public IDallas
{
public:
    MOCK_METHOD(void, requestCurrentTemperatures, (), (override));
    MOCK_METHOD(uint8_t, getNumberOfConnectedSensors, (), (override));
    MOCK_METHOD(float, getTemperatureByID, (uint8_t), (override));
    // MOCK_METHOD(void, getStringAddressByID, (uint8_t, char *), (override));
};
