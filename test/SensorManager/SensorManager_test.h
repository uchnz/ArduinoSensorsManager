#include <gtest/gtest.h>

#include <./../../test/MockIEth/MockIEth.h>
#include <./../../test/MockIMQTT/MockIMQTT.h>
#include <./../../test/MockIDallas/MockIDallas.h>
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
