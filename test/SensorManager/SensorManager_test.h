#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ISensor.h>
#include <IMQTT.h>
#include <SensorManager.h>

using ::testing::_;

class MockSensor : public ISensor
{
public:
   MOCK_METHOD(void, requestCurrentMeasurement, (), (override));
   MOCK_METHOD(uint8_t, getNumberOfConnectedSensors, (), (override));
   MOCK_METHOD(double, getCurrentMeasurementByID, (uint8_t), (override));
   MOCK_METHOD(uint8_t, getName, (char *), (override));
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
