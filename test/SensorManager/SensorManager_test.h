#include <gtest/gtest.h>

#include <MockIMQTT/MockIMQTT.h>
#include <MockIDallas/MockIDallas.h>
#include <SensorManager.h>

using ::testing::_;

class SensorManagerTest : public ::testing::Test
{
protected:
   void SetUp() override
   {
   }

   MockIMQTT _mqtt;
   SensorManager _mgr{_mqtt};
};

class SMTest2 : public ::testing::Test
{
protected:
   void SetUp() override
   {
   }

   MockIMQTT _mqtt;
   SensorManager _mgr{_mqtt};
};
