#pragma once

#include <gmock/gmock.h>
#include <IDallas.h>


namespace sensor_manager
{
    class MockIDallas : public IDallas
    {
    public:
        MOCK_METHOD(void, init, (), (override));
        MOCK_METHOD(void, setSensorsPrecision, (int), (override));
        MOCK_METHOD(void, requestCurrentTemperatures, (), (override));
        MOCK_METHOD(uint8_t, getNumberOfSensors, (), (override));
        MOCK_METHOD(float, getTemperatureByID, (uint8_t), (override));
        //        MOCK_METHOD(bool, updateTemperature, (), (override));
        MOCK_METHOD(void, getStringAddressByID, (uint8_t, char *), (override));
        // MOCK_METHOD(void, getStringAddressByID, (uint8_t, int *), (override));
    };
}