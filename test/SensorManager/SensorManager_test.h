#include <gtest/gtest.h>

#include <MockIEth/MockIEth.h>
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

   sensor_manager::MockIEth _eth;
   sensor_manager::MockIMQTT _mqtt;
   sensor_manager::MockIDallas _dallas;
   sensor_manager::SensorManager _mgr{_eth, _mqtt, _dallas};
};
