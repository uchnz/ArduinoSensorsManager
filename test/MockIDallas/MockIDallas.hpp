#include <gmock/gmock.h>
#include <stdint.h>
#include <IDallas.h>

namespace sensor_manager
{
    class MockIDallas : public IDallas
    {
    public:
        MOCK_METHOD(void, initSensors, (), (override));
        MOCK_METHOD(void, setSensorsPrecision, (int), (override));
        MOCK_METHOD(void, requestCurrentTemperatures, (), (override));
        MOCK_METHOD(uint8_t, getNumberOfSensors, (), (override));
        MOCK_METHOD(float, getTemperatureByID, (uint8_t), (override));
        MOCK_METHOD(char *, getStringAddressByID, (uint8_t), (override));
    };
}