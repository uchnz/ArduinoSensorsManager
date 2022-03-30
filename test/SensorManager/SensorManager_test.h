#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ISensor.h>
#include <IMQTT.h>
#include <SensorManager.h>

using ::testing::_;

class MockIDallas : public ISensor
{
public:
   MOCK_METHOD(void, requestCurrentTemperatures, (), (override));
   MOCK_METHOD(uint8_t, getNumberOfConnectedSensors, (), (override));
   MOCK_METHOD(float, getTemperatureByID, (uint8_t), (override));
};

class MockIMQTT : public IMQTT
{
public:
   MOCK_METHOD(bool, send, (const char *, const char *), (override));
   MOCK_METHOD(bool, receive, (), (override));
};

class SensorManagerTest : public ::testing::Test
{
protected:
   void SetUp() override
   {
   }

   MockIMQTT _mqtt;
   SensorManager _mgr{_mqtt};
};
