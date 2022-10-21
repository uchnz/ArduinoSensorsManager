#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ISensor.h>
#include <IActuator.h>
#include <IIOSensor.h>
#include <IMQTT.h>
#include <SensorManager.h>

using ::testing::_;

class MockSensor : public ISensor
{
public:
   MOCK_METHOD(bool, requestCurrentMeasurement, (), (override));
   MOCK_METHOD(uint8_t, getNumberOfConnectedSensors, (), (override));
   MOCK_METHOD(double, getCurrentMeasurementByID, (uint8_t), (override));
   MOCK_METHOD(uint8_t, getName, (char *), (override));
};

class MockActuator : public IActuator
{
public:
   MOCK_METHOD(bool, execute, (const char *), (override));
   MOCK_METHOD(uint8_t, getName, (char *), (override));
   MOCK_METHOD(uint8_t, getState, (char *), (override));
};

class MockIMQTT : public IMQTT
{
public:
   MOCK_METHOD(bool, send, (const char *, const char *), (override));
   MOCK_METHOD(bool, receive, (), (override));
   MOCK_METHOD(bool, subscribeToTopic, (const char *), (override));
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
